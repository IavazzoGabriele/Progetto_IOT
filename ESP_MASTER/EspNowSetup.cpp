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
}

