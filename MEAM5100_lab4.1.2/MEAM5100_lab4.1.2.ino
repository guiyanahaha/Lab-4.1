#define LED_channel 0 // use first of 6 channels
#define LED_freq 5  //LED frequency
#define LED_resolution_bits 14 //LED resolution in bits
#define LED_resolution ((1<<LED_resolution_bits)-1)

int value, value_map;
const int PotentiometerPin = 1;  // GPIO pin for the potentiometer
const int LEDPin = 4;     // GPIO pin for the LED

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  uint32_t duty = LED_resolution * min(value, valueMax) / valueMax;
  ledcWrite(channel, duty); // write duty to LEDC
}

void setup() {
  Serial.begin(9600);
  pinMode(PotentiometerPin, INPUT_PULLUP);  // Enable internal pull-up resistor for the switch
  ledcSetup(LED_channel, LED_freq, LED_resolution_bits); // setup LED pin
  ledcAttachPin(LEDPin, LED_channel);
}

void loop() {
  value = analogRead(PotentiometerPin); //read from potentiometer output
  value_map = map(value,0 , 4095, 0, 255);
  Serial.println(value_map);  //print value to check the range
  ledcAnalogWrite(LED_channel, value_map);  //write to LED
}
