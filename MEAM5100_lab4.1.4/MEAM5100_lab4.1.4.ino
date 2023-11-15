#include <WiFi.h>
#include <WiFiUdp.h>
#include "Webpage.h"
#include "html510.h"
HTML510Server h(80);

#define Motor_channel1 0 
#define Motor_channel2 1 

#define Motor_freq 1000  //Motor frequency
#define Motor_resolution_bits 8 //Motor resolution in bits
#define Motor_resolution ((1<<Motor_resolution_bits)-1)

WiFiServer server(80);

const char* ssid = "TP-Link_E0C8";  
const char* password = "52665134";   
char numberArray[20];
int recorder;

int value_map;
//const int PotentiometerPin = 1;  // GPIO pin for the potentiometer
const int MotorPin1 = 1;     // GPIO pin for the Motor Pin1
const int MotorPin2 = 10;     // GPIO pin for the Motor Pin2

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  uint32_t duty = Motor_resolution * min(value, valueMax) / valueMax;
  ledcWrite(channel, duty); // write duty to Motor pin
}

void setup() {
  ledcSetup(Motor_channel1, Motor_freq, Motor_resolution_bits); // setup Motor channel1
  ledcSetup(Motor_channel2, Motor_freq, Motor_resolution_bits); // setup Motor channel2
  ledcAttachPin(MotorPin1, Motor_channel1);
  ledcAttachPin(MotorPin2, Motor_channel2);
  
  IPAddress myIP(192,168,1,130);
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  WiFi.config(myIP,IPAddress(192,168,1,1),IPAddress(255,255,255,0));
  
  while(WiFi.status()!= WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("connected to %s on",ssid); 
  Serial.println(myIP);

  h.begin();
  h.attachHandler("/",handleRoot);
  h.attachHandler("/setDirection?val=", handleSlider1);
  h.attachHandler("/setDutyCycle?val=", handleSlider2);
}

void handleRoot(){
  h.sendhtml(body);
}

void handleSlider1() {
  int sliderValue = h.getVal();
  String s = "Move";
  if (sliderValue == 1) {
    s = s+ " Forward";
    ledcAnalogWrite(Motor_channel2,0);
    Serial.println(sliderValue);
  }
  else if(sliderValue == 3) {
    s = s+ " Backward";
    ledcAnalogWrite(Motor_channel1,0);
    Serial.println(sliderValue);
  }
  else if(sliderValue == 2) {
    s = s+ " Stop";
    ledcAnalogWrite(Motor_channel1,0);
    ledcAnalogWrite(Motor_channel2,0);
    Serial.println(sliderValue);
  }
  recorder = sliderValue;
  h.sendplain(s);
}

void handleSlider2() {
  int sliderValue = h.getVal();
  Serial.println(sliderValue);
  value_map = map(sliderValue,0 , 100, 0, 255);
  if (recorder == 1){
    ledcAnalogWrite(Motor_channel1,value_map);  // update Motor duty cycle
  } else if (recorder == 3){
    ledcAnalogWrite(Motor_channel2,value_map);
  }
  h.sendplain(itoa(sliderValue,numberArray,10)); //print slider value to website
}

void loop() {
  h.serve();
  delay(20);
}
