#include <Arduino.h>
#include <ESPSupabase.h>
#include <format>
#include <WiFi.h>

#define ID_USER 995
Supabase db;
String table = "USER"; //name of the table on the DB

// Put your supabase URL and Anon key here...
String supabase_url = "https://jideijhbzkdfiysrumbj.supabase.co";
String anon_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImppZGVpamhiemtkZml5c3J1bWJqIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzQ0NDc4OTQsImV4cCI6MjA5MDAyMzg5NH0.tUne5Z9m69PumLcQt0XCYbhwbMwyuTn25stMRZQqZmM";

// put your WiFi credentials (SSID and Password) here
const char *ssid = "TOPNET_728";
const char *psswd = "ahmed2004ilahi";

/*function that checks if the USER already exist in the DB
it loads the data from the DB, if the string=="" then the user do not exist on the DB
*/
bool existing() {
  String read = db.from(table).select("*").eq("id", ID_USER).limit(1).doSelect();
  return read.length() != 0;
}

/*function that inserts the USER in the DB
it prepares the json string then it inserts it in the DB
*/
String JSON = std::format("[{"id":{},"heart_rate":0,"SpO2":0,"temperature":37,"status":"STABLE","longitutde":0,"latitude":0,"name":{}" \n"}]", ID_USER,"ALI");
bool upsert = false;
void insert(){
  int code = db.insert(table, JSON, upsert);
  Serial.println(code);
  db.urlQuery_reset();
}

void setup()
{
  Serial.begin(115200);

  // Connecting to Wi-Fi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, psswd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  // Beginning Supabase Connection
  db.begin(supabase_url, anon_key);

  if(!existing()){
    insert();
  }

}

/*function that updates the USER's data in the DB
it prepares the json string then it updates it on the DB
*/
void update() {
  JSON = format([{"id":{},"heart_rate":{},"SpO2":{},"temperature":{},"status":{},"longitutde":{},"latitude":{},"name":"ALI \n"}],ID_USER,get_heartRate(),get_SpO2(),get_temperature(),get_status(),get_longitude(),get_latitude());
  int code = db.update(table).eq("id", ID_USER).doUpdate(JSON);
  Serial.println(code);
  db.urlQuery_reset();
}

void loop()
{
  update()
  delay(90000); //every 60seconds upldate
}