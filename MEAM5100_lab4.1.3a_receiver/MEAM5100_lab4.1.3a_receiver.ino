// Sending message to specific IPaddress
// Needs router and static IP
#include <WiFi.h>
#include <WiFiUdp.h>

#define LED_channel 0 // use first of 6 channels
#define LED_freq 5  //LED frequency
#define LED_resolution_bits 14 //LED resolution in bits
#define LED_resolution ((1<<LED_resolution_bits)-1)

const char* ssid = "TP-Link_E0C8";
const char* password = "52665134";
WiFiUDP UDPTestServer;
IPAddress myIP(192, 168, 1, 130);

int value, value_map;
const int LEDPin = 4;     // GPIO pin for the LED
const int UDP_PACKET_SIZE = 100; // allow packets up to 100 bytes
byte packetBuffer[UDP_PACKET_SIZE]; // can be up to 65535

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  uint32_t duty = LED_resolution * min(value, valueMax) / valueMax;
  ledcWrite(channel, duty); // write duty to LEDC
}

void handleUDPServer() {
  int cb = UDPTestServer.parsePacket();
  if (cb) {
    int len = UDPTestServer.read(packetBuffer, UDP_PACKET_SIZE-1);
    packetBuffer[len] = 0;
    Serial.printf("%s\n",packetBuffer);
  }
}

void setup() {
  ledcSetup(LED_channel, LED_freq, LED_resolution_bits); // setup LED pin
  ledcAttachPin(LEDPin, LED_channel);
  
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.config(myIP, IPAddress(192, 168, 1, 1),IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);
  UDPTestServer.begin(2808);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  handleUDPServer();
  delay(1);
  value = String((char*)packetBuffer).toInt(); //read from potentiometer output of sender
  Serial.println(value);
  value_map = map(value,0 , 4095, 0, 255);
  ledcAnalogWrite(LED_channel, value_map);  //write to LED
}
