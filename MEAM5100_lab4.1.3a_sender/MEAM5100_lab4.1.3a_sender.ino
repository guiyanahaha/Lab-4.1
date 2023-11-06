/ Sending message to specific IPaddress
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

IPAddress TargetIP(192, 168, 1, 123);
IPAddress myIP(192, 168, 1, 130);

char udpBuffer[100]; // expect messages upto 100 characters
char numberArray[20];

int value, value_map;
const int PotentiometerPin = 1;  // GPIO pin for the potentiometer

const int UDP_PACKET_SIZE = 100; // allow packets up to 100 bytes
byte packetBuffer[UDP_PACKET_SIZE]; // can be up to 65535

void fncUdpSend()
{
  // send what ever you want upto buffer size
  UDPTestServer.beginPacket(TargetIP, 2808); // send to UDPport 2808
  UDPTestServer.printf("%s",udpBuffer);
  UDPTestServer.endPacket();
  Serial.println(udpBuffer);
}

void setup() {
  pinMode(PotentiometerPin, INPUT_PULLUP);  // Enable internal pull-up resistor for the potentiometer
  
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
  value = analogRead(PotentiometerPin); //read from potentiometer output
  delay (1);
  strcpy(udpBuffer, itoa(value,numberArray,10));
  fncUdpSend();
}
