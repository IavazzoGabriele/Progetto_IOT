#include "EspNowSetup.h"



void printData(struct_message* msg){
  Serial.println(msg->temp);
  Serial.println(msg->hum);
  Serial.println(msg->bri);
  Serial.println(msg->bat);
  Serial.println(msg->soil);
  Serial.println(msg->water);
  Serial.println(msg->irr);
}



String parseString(msg_received* data){
  String d = "";
  d += String(msg->temp);
  d += ",";
  d += String(msg->hum);
  d += ",";
  d += String(msg->bri);
  d += ",";
  d += String(msg->bat);
  d += ",";
  d += String(msg->soil);
  d += ",";
  d += String(msg->water);
  d += ",";
  d += String(msg->irr);
  // d += "\n"; viene aggiunto durante file.println();
  return d;
}

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  printData(&incomingReadings);
  String data = parseString(&incomingReadings);
  writeDataOnSDCard(data, "masterData.csv");
}

