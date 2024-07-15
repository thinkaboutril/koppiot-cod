#include <SoftwareSerial.h>

SoftwareSerial scanner(0, 1); // RX, TX

void setup() {
  Serial.begin(9600);  // Set baud rate for communication with computer
  scanner.begin(9600);  // Set baud rate for communication with scanner
}

void loop() {
  if (scanner.available()) {
    String barcode = scanner.readStringUntil('\n');
    Serial.println(barcode);
  }
}
