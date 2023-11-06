#include <WiFi.h>
#include <WiFiUdp.h>
#include "Webpage.h"
#include "html510.h"
HTML510Server h(80);

#define LED_channel 0 // use first of 6 channels
#define LED_freq 5  //LED frequency
#define LED_resolution_bits 14 //LED resolution in bits
#define LED_resolution ((1<<LED_resolution_bits)-1)

WiFiServer server(80);

const char* ssid = "TP-Link_E0C8";  
const char* password = "52665134";   
char numberArray[20];

int value_map;
//const int PotentiometerPin = 1;  // GPIO pin for the potentiometer
const int LEDPin = 4;     // GPIO pin for the LED

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  uint32_t duty = LED_resolution * min(value, valueMax) / valueMax;
  ledcWrite(channel, duty); // write duty to LEDC
}

void setup() {
  //pinMode(PotentiometerPin, INPUT_PULLUP);  // Enable internal pull-up resistor for the switch
  ledcSetup(LED_channel, LED_freq, LED_resolution_bits); // setup LED pin
  ledcAttachPin(LEDPin, LED_channel);

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
  h.attachHandler("/setFrequency?val=", handleSlider1);
  h.attachHandler("/setDutyCycle?val=", handleSlider2);
}

void handleRoot(){
  h.sendhtml(body);
}

void handleSlider1() {
  int sliderValue = h.getVal();
  Serial.println(sliderValue);
  ledcSetup(LED_channel, sliderValue, LED_resolution_bits); // setup LED pin
  ledcAttachPin(LEDPin, LED_channel);
  h.sendplain(itoa(sliderValue,numberArray,10)); //print slider value to website
}

void handleSlider2() {
  int sliderValue = h.getVal();
  Serial.println(sliderValue);
  value_map = map(sliderValue,0 , 100, 0, 255);
  ledcAnalogWrite(LED_channel,value_map);  // update lED duty cycle
  h.sendplain(itoa(sliderValue,numberArray,10)); //print slider value to website
}

void loop() {
  h.serve();
  delay(20);
}
