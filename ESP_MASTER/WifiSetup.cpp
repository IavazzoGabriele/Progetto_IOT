#include "WifiSetup.h"
// Replace with your network credentials
//const char* ssid = "TIM-49495222"; //Replace with your own SSID
//const char* password = "yGZPQ6yCyXtGK2zAkUcRDSdQ"; //Replace with your own password
//const char* ssid = "Galaxy A14"; //Replace with your own SSID
//const char* password = "ciaociao"; //Replace with your own password

const char* ssid     = "ESP8266-Access-Point";
const char* password = "123456789";
 
void connectWifi(){
  WiFi.mode(WIFI_AP_STA);  //WIFI_AP_STA
  // WiFi.begin(ssid, password);
  WiFi.softAP(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi..");
  // }
  // Print ESP32 Local IP Address
  Serial.println("Connected to WiFi successfully");
  Serial.println(WiFi.softAPIP());
  Serial.println(WiFi.localIP());
}