// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
 
int updown;
int leftright;
 
 
const int low_joystick = 0;
const int high_joystick = 4095;
const float low_x = -7362.05;
const float high_x = 7362.05;
const float low_y = -14722.43;
const float high_y = 14722.43;
const int offset = 1300;
 
#define IN1A 18
#define IN2A 19
#define IN1B 22
#define IN2B 23
#define IN1C 25
#define IN2C 26
 
// Define a data structure
typedef struct struct_message {
  int x_value;
  int y_value;
  int sw_value;
 
} struct_message;
 
// Create a structured object
struct_message myData;
 
 
// Callback function executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  /*Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("x_value:");
  Serial.println(myData.x_value);
  Serial.print("y_value: ");
  Serial.println(myData.y_value);
  Serial.print("sw_value: ");
  Serial.println(myData.sw_value);
  Serial.println();*/
  updown = myData.x_value;
  leftright = myData.y_value;
  int xval = map(updown, low_joystick, high_joystick, low_x, high_x);
  int yval = map(leftright, low_joystick, high_joystick, low_y, high_y);
  /*Serial.print("x_value:");
  Serial.println(xval);
  Serial.print("y_value: ");
  Serial.println(yval);*/
  if (xval > -offset && xval < offset) {
    xval = 0;
  }
 
  if (yval > -offset && yval < offset) {
    yval = 0;
  }
 
  float u1 = xval / 50;                              //motor1 speed
  float u3 = ((-0.5 * xval) - (0.866 * yval)) / 50;  //motor2 speed
  float u2 = ((-0.5 * xval) + (0.866 * yval)) / 50;  //motor3 speed
 
  /*Serial.println(u1);
  Serial.println(u2);
  Serial.println(u3);
  delay(1000);*/
  if (u1 > 0) {
   
    analogWrite(IN1A, abs(u1));
    analogWrite(IN2A, 0);
    //analogWrite(motor1pinpwm, abs(u1));
  } else {
    int u11= abs(u1);
    analogWrite(IN1A, 0);
    analogWrite(IN2A, abs(u1));
    //analogWrite(motor1pinpwm, abs(u1));
  }
  if (u3 > 0) {
    //analogWrite(motor2pinpwm, abs(u3));
    analogWrite(IN1C, abs(u3));
    analogWrite(IN2C, 0);
  } else {
    //analogWrite(motor2pinpwm, abs(u3));
    analogWrite(IN1C, 0);
    analogWrite(IN2C, abs(u3));
  }
  if (u2 > 0) {
   
    //analogWrite(motor3pinpwm, abs(u2));
    analogWrite(IN1B, abs(u2));
    analogWrite(IN2B, 0);
  } else {
   
    //analogWrite(motor3pinpwm,abs(u2));
    analogWrite(IN1B, 0);
    analogWrite(IN2B, abs(u2));
  }
 
}
 
void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  pinMode(IN1A, OUTPUT);
  pinMode(IN2A, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2B, OUTPUT);
  pinMode(IN1C, OUTPUT);
  pinMode(IN2C, OUTPUT);
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  // Register callback function
}
 
void loop() {
}
has context menu
Compose