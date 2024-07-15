#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

SoftwareSerial scanner(D5, D6); // RX, TX (use suitable GPIO pins for RX and TX)

const int relayPin = D1; // Pin connected to the relay

// Wi-Fi credentials
const char* ssid = "Kmila 2";
const char* password = "YOUR_PASSWORDb312uang";

// Google Apps Script web app URL
const char* serverName = "YOUR_GOOGLE_APPS_SCRIPT_WEB_APP_URL";

WiFiClient wifiClient;

void setup() {
  Serial.begin(9600);  // Set baud rate for communication with computer
  scanner.begin(9600);  // Set baud rate for communication with scanner

  pinMode(relayPin, OUTPUT);  // Set relay pin as output
  digitalWrite(relayPin, LOW);  // Ensure relay is off initially

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  if (scanner.available()) {
    String barcode = scanner.readStringUntil('\n');
    Serial.println(barcode);

    // Check barcode against Google Sheets data
    if (checkBarcodeInSheet(barcode)) {
      // Energize relay for 5 seconds
      digitalWrite(relayPin, HIGH);
      delay(5000);  // Delay for 5 seconds
      digitalWrite(relayPin, LOW);
    } else {
      Serial.println("Barcode not found in Google Sheets");
    }
  }
}

bool checkBarcodeInSheet(String barcode) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(serverName) + "?barcode=" + barcode;
    http.begin(wifiClient, url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      http.end();
      return payload == "match";
    } else {
      Serial.println("Error on HTTP request");
      http.end();
      return false;
    }
  } else {
    Serial.println("Wi-Fi not connected");
    return false;
  }
}
