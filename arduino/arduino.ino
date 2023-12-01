#include <SoftwareSerial.h>

#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

	
#define BUZZER 5
#define MoistureSensor A0

int MoistureValue;

int step_number = 0;
const int stepPerRev = 1024;

SoftwareSerial s(5,6);

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(MoistureSensor, INPUT);

  Serial.begin(9600);
  s.begin(9600);
}

void loop() {
  noTone(BUZZER);
  MoistureValue = analogRead(MoistureSensor);
  Serial.println("MoistureValue");
  Serial.println(MoistureValue);

  if(s.available() > 0) {
    char c = s.read();
    Serial.println(c);
    if (c == 'f') {
      tone(BUZZER, 85);
      delay(1000);
      int data = 1;
      rotateForward();
      s.write(data);
    } else if (c == 'b') {
      
      int data = 0;
      rotateBack();
      s.write(data);
    }
  }
  Serial.println("END");
  delay(1000);
}

void rotateForward() {
  for(int a = 0; a < 1000; a++) {
    OnStep(false);
    delay(2);
  }
}

void rotateBack() {
  for(int a = 0; a < 1000; a++) {
    OnStep(true);
    delay(2);
  }
}

void OnStep(bool dir) {
  if(dir)
  {
    switch(step_number) {
      case 0:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
      case 1:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
      case 2:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
      case 3:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
    }
  }
  else
  {
    switch(step_number) {
      case 0:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;
      case 1:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
      case 2:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
      case 3:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    }
  }
  step_number++;
  if(step_number > 3){
    step_number = 0;
  }
}
