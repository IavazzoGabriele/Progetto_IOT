#include <SoftwareSerial.h> // Comunicazione seriale Arduino-esp8266

SoftwareSerial espSerial(10, 11); // Pines modificati per evitare conflitti con Serial

#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0x8c, 0xaa, 0xb5, 0x7c, 0x6a, 0x17};

// Define variables to store readings to be sent
float temperature;
float humidity;
float bright;
float batteryLevel;
float soilHumidity;
float waterLevel;
float irrigationMode;

// Updates readings every 10 seconds
const long interval = 10000; 
unsigned long previousMillis = 0;    // will store last time readings were updated 

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    float temp;
    float hum;
    float bri;
    float bat;
    float soil;
    float water;
    float irr;
} struct_message;

// Create a struct_message called DHTReadings to hold sensor readings
struct_message readings;

typedef struct struct_msg {
  char mode;  //d = default, a = auto, m = manual
  unsigned long humidity_thresh;  //solo per a
  String datetime;  //solo per d
} msg_sent;

msg_sent incomingReadings;

void printData(msg_sent* msg){
  Serial.println(msg->humidity_thresh);
  Serial.println(msg->datetime);
  Serial.println(msg->mode);
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

/*void getReadings(){
  // legge i valori da trasmettere poi all'altro esp
  if (espSerial.available()) {
    String dati = Serial.readString();
    Serial.println(dati);
  }
  char dataArray[dati.length() + 1];
  dati.toCharArray(dataArray, dati.length() + 1);
  char* token = strtok(dataArray, ",");
  int i = 0;
  float floatArray[7]; // adjust the size according to your needs

  while (token!= NULL) {
    floatArray[i] = atof(token);
    token = strtok(NULL, ",");
    i++; 
  }
  readings.water = waterLevel;
  readings.bat = batteryLevel;
  readings.bri = bright;
  readings.temp = temperature;
  readings.hum = humidity;
  readings.soil = soilHumidity;
  readings.irr = irrigationMode;
}*/

const char* ssid = "Sky Wifi";  // SSID della rete Wi-Fi
const char* password = "federicogiuliafedericogiulia";  // Password della rete Wi-Fi

void collegaWifi() {
  // Inizializza la comunicazione seriale
  // Avvia la connessione Wi-Fi
  Serial.print("Connessione a ");
  Serial.println(ssid);


  WiFi.begin(ssid, password);

  // Attendi fino alla connessione
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connesso");
  Serial.println("Indirizzo IP: ");
  Serial.println(WiFi.localIP());
}

void creaDatiFasulli() {
  readings.water = 1;
  readings.bat = 2;
  readings.bri = 3;
  readings.temp = 4;
  readings.hum = 5;
  readings.soil = 6;
  readings.irr = 7;
}

void setup() {
  // Init Serial Monitor
  Serial.begin(9600);
  espSerial.begin(9600);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;

    //Get DHT readings
    //getReadings();
    creaDatiFasulli();
    
    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &readings, sizeof(readings));
  }
}