#include <WiFi.h>
#include <WebServer.h>
#include "html.h"

WebServer server(80);

String ssid = "";
String password = "";

void handleRoot() {
  String html = html_code;
  server.send(200, "text/html", html);
}

void handleConnect() {
  ssid = server.arg("ssid");
  password = server.arg("pwd");

  server.send(200, "text/html", "Trying to connect...");

  WiFi.begin(ssid.c_str(), password.c_str());

  for (int i = 0; i < 10; i++) {
    delay(1000);
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected!");
      return;
    }
  }

  Serial.println("Failed to connect");
}

void startAP() {
  WiFi.softAP("Pico-Setup", "12345678");
  if (!WiFi.softAP("Pico-Setup", "12345678")) {
    Serial.println("AP FAILED");
  }

  delay(1000); // IMPORTANT

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
}

void setup() {
  Serial.begin(115200);

  if (ssid == "" || password == "") {
    startAP();
    Serial.println("Starting server...");
  } else {
    WiFi.begin(ssid.c_str(), password.c_str());
  }

  server.on("/test", []() {
    server.send(200, "text/plain", "Server works");
  });
  server.on("/", handleRoot);
  server.on("/connect", handleConnect);

  server.begin();
}

void loop() {
  server.handleClient();
}