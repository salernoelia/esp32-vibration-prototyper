#include <Arduino.h>

const int motorPin = 16;
const int buttonPin = 17;
const int OTB_Pin = 36;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 20;
int lastButtonState = HIGH;
int buttonState = HIGH;

const int freq = 5000;
const int pwmChannel = 0;
const int resolution = 8;

bool motorOn = false;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(motorPin, pwmChannel);
  ledcWrite(pwmChannel, 0);
  analogReadResolution(12);
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay && reading != buttonState) {
    buttonState = reading;
    if (buttonState == LOW) {
      motorOn = !motorOn; 
    }
  }

  lastButtonState = reading;

  if (motorOn) {
    int potValueB = analogRead(OTB_Pin);
    int duty = map(potValueB, 0, 4095, 50, 255);
    ledcWrite(pwmChannel, duty);
  } else {
    ledcWrite(pwmChannel, 0);
  }
}
