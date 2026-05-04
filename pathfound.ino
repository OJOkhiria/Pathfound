#include <LedControl.h>

int BUT1 = D0;
int BUT2 = D1;
int BUT3 = D2;

int LED1 = D3;
int LED2 = D4;
int LED3 = D5;

int DIN_PIN = D10;
int CLK_PIN = D9;
int CS_PIN  = D8;

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

int count = 0;

int lastIncReading = HIGH;
int incState = HIGH;
unsigned long lastIncDebounceTime = 0;

int lastResetReading = HIGH;
int resetState = HIGH;
unsigned long lastResetDebounceTime = 0;

const unsigned long debounceDelay = 50;

void displayCount(int value) {
  int tens = value / 10;
  int ones = value % 10;

  // Many MAX7219 2-digit modules show digit 0 as the rightmost digit
  lc.setDigit(0, 0, ones, false);
  lc.setDigit(0, 1, tens, false);
}


void setup() {
  // put your setup code here, to run once

pinMode(BUT1, INPUT_PULLUP);

pinMode(BUT2, INPUT_PULLUP);

pinMode(BUT3, INPUT_PULLUP);


pinMode(LED1, OUTPUT);

pinMode(LED2, OUTPUT);

pinMode(LED3, OUTPUT);
lc.shutdown(0, false);
lc.setIntensity(0, 10);   // 0 to 15
lc.clearDisplay(0);
displayCount(count);

long startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
long but1time;
long but2time;
long but3time = 0;
unsigned long now = millis();
if (digitalRead(BUT1) == HIGH) {
but1time = millis(); 
digitalWrite(LED1, HIGH);
} else {
digitalWrite(LED1, LOW);
}

if (digitalRead(BUT2) == HIGH) {
but2time = millis();
}

if (abs(but2time - but1time) <= 2000) {
digitalWrite(LED2, HIGH);
} else {
digitalWrite(LED2, LOW);
}

if (digitalRead (BUT3) == HIGH) {
but3time++;
} else {
but3time = 0 ;
}

if (but3time >= 800 ) {
digitalWrite (LED3, HIGH);
} else {
digitalWrite (LED3, LOW);
}

int incReading = digitalRead(BUT1);
if (incReading != lastIncReading) {
lastIncDebounceTime = now;
}
if ((now - lastIncDebounceTime) > debounceDelay) {
    if (incReading != incState) {
      incState = incReading;
      if (incState == LOW) {
        count = (count + 1) % 100;
        displayCount(count);
      }
    }
  }
lastIncReading = incReading;

  int resetReading = digitalRead(BUT2);
  if (resetReading != lastResetReading) {
    lastResetDebounceTime = now;
  }
  if ((now - lastResetDebounceTime) > debounceDelay) {
    if (resetReading != resetState) {
      resetState = resetReading;
      if (resetState == LOW) {
        count = 0;
        displayCount(count);
      }
    }
  }
  lastResetReading = resetReading;
}
