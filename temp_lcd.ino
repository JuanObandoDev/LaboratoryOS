#include <LiquidCrystal.h>
const int LED_RED = 13;
const int FAN1 = 10;
int seconds = 0;
LiquidCrystal lcd_1(12, 11, 5, 4, 3, 2);

int sensor_value;
float temperature;

void setup() {
  lcd_1.begin(16, 2);
  //lcd_1.print("hello world");
  pinMode(LED_RED, OUTPUT);
  pinMode(FAN1, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  readTemperature();
  displayTemperature();
  handleTemperatureResponse();
}

void readTemperature() {
  sensor_value = analogRead(A0);
  temperature = (((sensor_value * 5.0 / 1023) - 0.5) * 100);
}

void displayTemperature() {
  Serial.print("Sensor value: ");
  Serial.println(temperature);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Sensor Value:");
  lcd_1.print(temperature);
}

void handleTemperatureResponse() {
  if (temperature <= 10) {
    sendWarningMessage("Cold Temp       ");
    turnOffFan();
    blinkLed(LED_RED, 500);
  } else if (temperature >= 11 && temperature <= 25) {  
    sendWarningMessage("Normal Temp     ");
    turnOffFan();
    turnOffLed(LED_RED);
  } else if (temperature >= 26){  
    sendWarningMessage("Critical Temp!! ");
    turnOnFan();
    turnOnLed(LED_RED);
  }
}

void turnOffFan() {
  digitalWrite(FAN1, LOW);
}

void turnOnFan() {
  digitalWrite(FAN1, HIGH);
}

void blinkLed(int led, int delayTime) {
  digitalWrite(led, HIGH);
  delay(delayTime);
  digitalWrite(led, LOW);
  delay(delayTime);
}

void blinkFan(int fan, int delayTime) {
  digitalWrite(fan, HIGH);
  delay(delayTime);
  digitalWrite(fan, LOW);
}

void sendWarningMessage(const char* msg) {
  Serial.println(msg);
  lcd_1.setCursor(0, 1);
  lcd_1.print(msg);
}

void turnOnLed(int led) {
  digitalWrite(led, HIGH);
}

void turnOffLed(int led) {
  digitalWrite(led, LOW);
}
