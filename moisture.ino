#include <Arduino.h>
#include <FirebaseESP8266.h>
#if defined(ESP32)
#include <Wifi.h>
#elif defined(ESP8266)
#include <ESP8266Wifi.h>
#endif

// WIFI CREDENTIALS
#define WIFI_SSID "AndroidAP"
#define WIFI_PASSWORD "1234567890"

// Firebase CREDENTIALS
#define FIREBASE_HOST "https://iot-00011883-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "6MdgShButXbbt8592bqdtAxah2hx2PrS348f0sJR"

int input = 7;

int pin  = A0;
float sensorValue = 0;

void setup() {
	Serial.begin(9600);

  pinMode(input, OUTPUT);

  pinMode(pin, INPUT);

  digitalWrite(input, HIGH);

  delay(500);
}

void loop() {
  Serial.println("Moisture level: ");

  sensorValue = analogRead(pin);
  Serial.println(sensorValue);

  if (sensorValue  > 450) {
    digitalWrite(input, LOW);
  } else {
    digitalWrite(input, HIGH);
  }
  
  delay(5000);

}
