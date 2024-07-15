#include <SoftwareSerial.h>

SoftwareSerial scanner(D5, D6); // RX, TX (use suitable GPIO pins for RX and TX)

const int relayPin = D1; // Pin connected to the relay

void setup() {
  Serial.begin(9600);  // Set baud rate for communication with computer
  scanner.begin(9600);  // Set baud rate for communication with scanner
  
  pinMode(relayPin, OUTPUT);  // Set relay pin as output
  digitalWrite(relayPin, LOW);  // Ensure relay is off initially
}

void loop() {
  if (scanner.available()) {
    String barcode = scanner.readStringUntil('\n');
    Serial.println(barcode);
    
    // Energize relay for 5 seconds
    digitalWrite(relayPin, HIGH);
    delay(5000);  // Delay for 5 seconds
    digitalWrite(relayPin, LOW);
  }
}
