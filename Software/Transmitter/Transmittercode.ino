/*
  ESP-NOW Demo - Transmit
  esp-now-demo-xmit.ino
  Sends data to Responder
  
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/

//Include Libraries
#include <esp_now.h>
#include <WiFi.h>

// Variables for test data

int x = 32;
int y = 33;
int sw = 21;

// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = { 0xE8, 0x6B, 0xEA, 0xDE, 0xC5, 0x78 };

// Define a data structure
typedef struct struct_message {
  int x_value;
  int y_value;
  int sw_value;

} struct_message;

// Create a structured object
struct_message myData;

// Peer info
esp_now_peer_info_t peerInfo;

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {

  // Set up Serial Monitor
  Serial.begin(115200);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(21, INPUT_PULLUP);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the send callback
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {

  // Create test data

  // Format structured data

  myData.x_value = analogRead(32);
  myData.y_value = analogRead(33);
  myData.sw_value = digitalRead(21);

 

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  } else {
    Serial.println("Sending error");
  }
  Serial.print("x_value:");
  Serial.print(myData.x_value);

  Serial.print("y_value:");
  Serial.print(myData.y_value);

  Serial.print("sw_value:");
  Serial.println(myData.sw_value);
  delay(1000);
  
}
