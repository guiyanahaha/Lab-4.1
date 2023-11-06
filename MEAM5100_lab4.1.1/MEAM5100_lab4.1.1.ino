const int switchPin = 1;  // GPIO pin for the switch
const int ledPin = 4;     // GPIO pin for the LED

void setup() {
  pinMode(switchPin, INPUT_PULLUP);  // Enable internal pull-up resistor for the switch
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Read the state of the switch
  int switchState = digitalRead(switchPin);

  if (switchState == LOW) {
    // Switch is pressed (button is closed)
    digitalWrite(ledPin, HIGH);  // Turn on the LED
  } else {
    // Switch is not pressed (button is open)
    digitalWrite(ledPin, LOW);  // Turn off the LED
  }
}
