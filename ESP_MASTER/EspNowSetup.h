#include <stdint.h> 
#include <SoftwareSerial.h>

#ifndef ESPNOWSETUP_H
#define ESPNOWSETUP_H


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    float temp;
    float hum;
    float bri;
    float bat;
    float soil;
    float water;
    float irr;
} msg_received;


typedef struct struct_msg {
  char mode;  /*d = default, a = auto, m = manual*/
  unsigned long humidity_thresh;  /*solo per a*/
  String datetime;   /*solo per d*/
} msg_sent;





extern msg_sent dataToSend;
extern msg_received incomingReadings;


void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len);



#endif
