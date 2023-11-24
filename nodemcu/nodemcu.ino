#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <Stepper.h>
#include <SoftwareSerial.h>

#define _SSID "Netis"          // Your WiFi SSID
#define _PASSWORD "The/+Future+/_/-Rich-/Family"      // Your WiFi Password
#define REFERENCE_URL "https://iot-00011883-default-rtdb.firebaseio.com/"  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);

#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
uint8_t DHTPin = D1;
uint8_t LEDpin = D2;
uint8_t WATERpin = A0;
DHT dht(DHTPin, DHTTYPE);

float Temperature;
float Humidity; 
float Water;
int LedStatus;

SoftwareSerial s(D6,D5);
int data;

void setup() {
  s.begin(9600);
  Serial.begin(9600);
  delay(100);

  pinMode(DHTPin, INPUT);
  pinMode(LEDpin, OUTPUT);
  pinMode(WATERpin, INPUT);

  dht.begin(); 

  WifiSetup();
}

void WifiSetup() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected!");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  listenSerial();
  connectLed();
  connectTemp();
  connectWaterSensor();
  
  wateringCalculation();
  Serial.println("============END of LOOP=============");
  Serial.println();
  delay(1000);
}

void connectTemp() {
  float tData = firebase.getFloat("Temperature");
  float hData = firebase.getFloat("Humidity");

  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity

  Serial.println("Current Temperature: ");
  Serial.println(Temperature);
  Serial.println("Current Humidity: ");
  Serial.println(Humidity);

  if(Temperature != tData) {
    firebase.setFloat("Temperature", Temperature);
  }
  if(Humidity != hData) {
    firebase.setFloat("Humidity", Humidity);
  }
}

void connectWaterSensor() {
  Water = firebase.getFloat("WaterLevel");
  float waterValue = analogRead(WATERpin);
  Serial.println("Water: ");
  Serial.println(waterValue);
  if (Water != waterValue) {
    firebase.setFloat("WaterLevel", waterValue);
  }
}

void connectLed() {
  LedStatus = firebase.getInt("LedStatus");
  if(LedStatus == 1)
  {
    onLedOn();
  }
  else {
    onLedOff();
  }
}

void onLedOn() {
  firebase.setFloat("LedStatus", 1);
  digitalWrite(LEDpin, HIGH);
  Serial.println("Led Status: ON");
}

void onLedOff() {
  firebase.setFloat("LedStatus", 0);
  digitalWrite(LEDpin, LOW);
  Serial.println("Led Status: OFF");
}

void wateringCalculation() {
  if (Humidity <= 40.00) {
    onLedOn();
    s.write("f");
  } else {
    s.write("b");
    onLedOff();
  }
}

void listenSerial() {
  s.write("f");
  if (s.available() > 0) {
    data = s.read();
    Serial.println("MCU Recieved");
    Serial.println(data);
  }  
}