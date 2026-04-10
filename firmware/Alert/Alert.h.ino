#ifndef ALERT_H
#define ALERT_H

#include <Arduino.h>

// ================= CONFIG =================

// 📞 NUMEROS
const char* PHONE_NUMBERS[] = {
    "+216XXXXXXXX",
    "+216YYYYYYYY"
};

const int NUM_COUNT = sizeof(PHONE_NUMBERS) / sizeof(PHONE_NUMBERS[0]);

// ⏱️ Anti-spam
unsigned long ALERT_COOLDOWN = 60000;

// 🔁 Retry
int MAX_RETRY = 3;

// 🔋 Batterie (pin analog)
int BATTERY_PIN = 34; // ESP32
float MIN_BATTERY_VOLTAGE = 3.5;

// ================= TYPES =================
enum AlertType {
    FALL,
    HEART,
    OXYGEN,
    UNKNOWN
};

// ================= CLASSE =================

class Alert {
  private:
    HardwareSerial* gsmSerial;
    unsigned long lastAlertTime = 0;

  public:

    Alert(HardwareSerial &serial) {
      gsmSerial = &serial;
    }

    void begin(long baud = 9600) {
      gsmSerial->begin(baud);
      delay(1000);

      sendCommand("AT");
      sendCommand("AT+CMGF=1");
      sendCommand("AT+CNMI=1,2,0,0,0"); // recevoir SMS direct
    }

    void sendCommand(String cmd) {
      gsmSerial->println(cmd);
      delay(1000);
    }

    // ================= GPS =================
    String getGPSLocation() {
      // 👉 A remplacer par vrai module GPS
      float lat = 36.8065;
      float lon = 10.1815;

      return "https://maps.google.com/?q=" + String(lat,6) + "," + String(lon,6);
    }

    // ================= BATTERIE =================
    float getBatteryVoltage() {
      int value = analogRead(BATTERY_PIN);
      float voltage = (value / 4095.0) * 4.2; 
      return voltage;
    }

    bool isBatteryOK() {
      return getBatteryVoltage() >= MIN_BATTERY_VOLTAGE;
    }

    // ================= GSM =================
    bool waitForOK(unsigned long timeout = 5000) {
      unsigned long start = millis();
      String response = "";

      while (millis() - start < timeout) {
        while (gsmSerial->available()) {
          char c = gsmSerial->read();
          response += c;
        }
        if (response.indexOf("OK") != -1) return true;
      }
      return false;
    }

    // ================= SMS =================
    bool sendSMS(String number, String message) {

      for (int attempt = 0; attempt < MAX_RETRY; attempt++) {

        gsmSerial->println("AT+CMGF=1");
        delay(300);

        gsmSerial->print("AT+CMGS=\"");
        gsmSerial->print(number);
        gsmSerial->println("\"");
        delay(300);

        gsmSerial->print(message);
        delay(300);

        gsmSerial->write(26);

        if (waitForOK()) {
          Serial.println("SMS envoye !");
          return true;
        }

        Serial.println("Retry...");
        delay(2000);
      }

      return false;
    }

    void sendAlertToAll(String message) {
      for (int i = 0; i < NUM_COUNT; i++) {
        sendSMS(PHONE_NUMBERS[i], message);
      }
    }

    // ================= MESSAGE =================
    String buildMessage(AlertType type) {

      String msg;

      switch (type) {
        case FALL:
          msg = "ALERTE: Chute detectee!";
          break;
        case HEART:
          msg = "ALERTE: Probleme cardiaque!";
          break;
        case OXYGEN:
          msg = "ALERTE: Oxygene critique!";
          break;
        default:
          msg = "ALERTE: Probleme detecte!";
      }

      msg += "\nLocalisation: " + getGPSLocation();
      msg += "\nBatterie: " + String(getBatteryVoltage()) + "V";

      return msg;
    }

    // ================= ACK SMS =================
    void checkIncomingSMS() {
      if (gsmSerial->available()) {
        String msg = gsmSerial->readString();

        if (msg.indexOf("OK") != -1) {
          Serial.println("ACK recu utilisateur !");
        }

        if (msg.indexOf("STOP") != -1) {
          Serial.println("Alerte stoppee par utilisateur !");
        }
      }
    }

    // ================= TRIGGER =================
    bool trigger(bool state, AlertType type) {

      if (!state) return false;

      // 🔋 Batterie check
      if (!isBatteryOK()) {
        Serial.println("Batterie trop faible !");
        return false;
      }

      // ⛔ Anti-spam
      if (millis() - lastAlertTime < ALERT_COOLDOWN) {
        Serial.println("Cooldown actif");
        return false;
      }

      String message = buildMessage(type);

      sendAlertToAll(message);

      lastAlertTime = millis();

      return true;
    }
};

#endif
