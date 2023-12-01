#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <Stepper.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

#define _SSID "Netis"          // Your WiFi SSID
#define _PASSWORD "The/+Future+/_/-Rich-/Family"      // Your WiFi Password
#define REFERENCE_URL "https://iot-00011883-default-rtdb.firebaseio.com/"  // Your Firebase project reference url

Firebase firebase(REFERENCE_URL);

#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

LiquidCrystal_I2C lcd(0x3F,16,2);

uint8_t DHTPin = D3;
uint8_t LEDpin = D4;
uint8_t WATERpin = A0;
DHT dht(DHTPin, DHTTYPE);

// Data related with Firebase Values
float TemperatureValue;
float HumidityValue; 
float WaterValue;
float MoistureValue;
int LedStatusValue;

int isWateringOn;  // Local variable to watch status of working 1 - ON, 2 - OFF, 3 - STALE

SoftwareSerial s(D6,D5);
int data; // Recor of Arduino message or MoistureValue

unsigned long currentMillis;

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  delay(100);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(DHTPin, INPUT);
  pinMode(LEDpin, OUTPUT);
  pinMode(WATERpin, INPUT);

  dht.begin(); 

  WifiSetup();
  LCDSetup();
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
void LCDSetup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loadInitialValues() {
  TemperatureValue = firebase.getFloat("Temperature");
  HumidityValue = firebase.getFloat("Humidity");
  WaterValue = firebase.getInt("WaterLevel"); 
  LedStatusValue = firebase.getInt("LedStatus"); 
}

void loop() {
  loadInitialValues();

  watchLed();
  watchTemp();
  watchWaterSensor();

  listenSerial();
  
  wateringCalculation();
  
  Serial.println("============END of LOOP=============");
  Serial.println();

  delay(1000);
}

void watchTemp() {
  TemperatureValue = dht.readTemperature(); // Gets the values of the temperature
  HumidityValue = dht.readHumidity(); // Gets the values of the humidity
  firebase.setFloat("Temperature", TemperatureValue);
  firebase.setFloat("Humidity", HumidityValue);

  showDHTToLCD();

  Serial.println("Current Temperature: ");
  Serial.println(TemperatureValue);
  Serial.println("Current Humidity: ");
  Serial.println(HumidityValue);
}
void showDHTToLCD() {
  String firstLine = String("Temperature: ")+ String(TemperatureValue);
  String secondLine = String("Humidity: ")+ String(HumidityValue);
  lcd.setCursor(0, 0);
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
}

void watchWaterSensor() {
  WaterValue = analogRead(WATERpin);
  firebase.setFloat("WaterLevel", WaterValue);

  Serial.println("Water: ");
  Serial.println(WaterValue);
}

void watchLed() {
  if(LedStatusValue == 1) {
    onLedOn();
  }
  else if(LedStatusValue == 0) {
    onLedOff();
  }
}
void onLedOn() {
  digitalWrite(LEDpin, HIGH);
  Serial.println("Led Status: ON");
}
void onLedOff() {
  digitalWrite(LEDpin, LOW);
  Serial.println("Led Status: OFF");
}

void wateringCalculation() {
  if (WaterValue > 400) { // || MoistureValue > 600 raining or water poured, turn watering off
    firebase.setInt("LedStatus", 0); 
    firebase.setInt("EnableWatering", 0);
    isWateringOn = 0;
  } else if (TemperatureValue > 40 ) { // || MoistureValue < 400
    currentMillis = millis();
    Serial.println(currentMillis);
    String str =String("{ milis: ")+String(currentMillis)+String(", ")+String("value:")+String(1)+String(" }");
    Serial.println(str);
    firebase.pushString("WateringCounts", str);
    firebase.setInt("LedStatus", 1); 
    firebase.setInt("EnableWatering", 1);
    isWateringOn = 1;
  }
}

void listenSerial() {
  if(isWateringOn == 1) {
    s.write("f");
    isWateringOn = 3;
  } else if (isWateringOn == 0) {
    onLedOff();
    s.write("b");
    isWateringOn = 3;
  }
  if (s.available() > 0) {
    data = s.read();
    if (data == 1) {
      Serial.println("Watering Turned ON");
    } else if (data == 0) {
      Serial.println("Watering Turned OFF");
    }
  }  
}
