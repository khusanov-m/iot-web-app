#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include "DHT.h"

#define _SSID "Netis"          // Your WiFi SSID
#define _PASSWORD "The/+Future+/_/-Rich-/Family"      // Your WiFi Password
#define REFERENCE_URL "https://iot-00011883-default-rtdb.firebaseio.com/"  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);

#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
uint8_t DHTPin = D1;
uint8_t LEDpin = D2;
uint8_t WATERpin = D7;
// uint8_t MOISTUREpin = D4;
DHT dht(DHTPin, DHTTYPE);

float Temperature;
float Humidity; 
float Water;
// float Moisture;
int LedStatus;

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(DHTPin, INPUT);
  pinMode(LEDpin, OUTPUT);
  pinMode(WATERpin, INPUT);
  // pinMode(MOISTUREpin, INPUT);

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
  Serial.println("=============LED============");
  connectLed();
  Serial.println("=============DHT============");
  connectTemp();
  Serial.println("=============Water============");
  connectWater();
  //  Serial.println("=============Moisture============");
  // connectMoisture();
  Serial.println("=============wateringCalculation============");

  wateringCalculation();
  Serial.println("============END of LOOP=============");
  delay(1000);
}

void connectTemp() {
  float tData = firebase.getFloat("Temperature");
  float hData = firebase.getFloat("Humidity");

  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity

  Serial.println("Current Temperature: ");
  Serial.println(Temperature);
  Serial.println("Previous Temperature: ");
  Serial.println(tData);

  Serial.println("Current Humidity: ");
  Serial.println(Humidity);
  Serial.println("Previous Humidity: ");
  Serial.println(hData);

  firebase.setFloat("Temperature", Temperature);
  firebase.setFloat("Humidity", Humidity);
}

void connectWater() {
  Water = firebase.getFloat("WaterLevel");
  Serial.println("Water: ");
  Serial.println(Water);
  firebase.setFloat("WaterLevel", Water);
}

// void connectMoisture() {
//   Moisture = firebase.getFloat("MoistureLevel");
//   Serial.println("Moisture: ");
//   Serial.println(Moisture);
//   firebase.setFloat("MoistureLevel", Moisture);
// }

void connectLed() {
  LedStatus = firebase.getInt("LedStatus");
  Serial.println("LedStatus");
  Serial.println(LedStatus);

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
  Serial.println("D1 Status: ON");
}

void onLedOff() {
  firebase.setFloat("LedStatus", 0);
  digitalWrite(LEDpin, LOW);
  Serial.println("D1 Status: OFF");
}

void wateringCalculation() {
  if (Humidity > 40.00) {
    onLedOn();
  } else {
    onLedOff();
  }
}