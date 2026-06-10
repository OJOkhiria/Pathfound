#include <Arduino.h>

const int BUT1 = D0;  // Increment
const int BUT2 = D1;  // Decrement
const int BUT3 = D2;  // Reset (hold 800 ms)

const int LED1 = D3;
const int LED2 = D4;
const int LED3 = D5;

const int DIN_PIN = D10;
const int CLK_PIN = D8;
const int CS_PIN  = D7;

int count = 0;

// Increment debounce
int lastIncReading = HIGH;
int incState = HIGH;
unsigned long lastIncDebounceTime = 0;

// Decrement debounce
int lastDecReading = HIGH;
int decState = HIGH;
unsigned long lastDecDebounceTime = 0;

const unsigned long debounceDelay = 50;

// Timing variables
unsigned long lastBut1Time = 0;
unsigned long lastBut2Time = 0;
unsigned long but3PressStart = 0;

bool resetPerformed = false;

void sendToMax7219(uint8_t opcode, uint8_t data) {
  digitalWrite(CS_PIN, LOW);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, opcode);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, data);
  digitalWrite(CS_PIN, HIGH);
}

void initDisplay() {
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);

  digitalWrite(DIN_PIN, LOW);
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(CS_PIN, HIGH);

  sendToMax7219(0x0F, 0x00);
  sendToMax7219(0x0C, 0x01);
  sendToMax7219(0x0B, 0x01);
  sendToMax7219(0x09, 0xFF);
  sendToMax7219(0x0A, 0x08);

  sendToMax7219(1, 0x0F);
  sendToMax7219(2, 0x0F);
}

void clearDisplay() {
  sendToMax7219(1, 0x0F);
  sendToMax7219(2, 0x0F);
}

void showStartupTest() {
  sendToMax7219(1, 8);
  sendToMax7219(2, 8);
  delay(500);

  sendToMax7219(1, 1);
  sendToMax7219(2, 2);
  delay(500);

  clearDisplay();
}

void displayCount(int value) {
  value = constrain(value, 0, 99);

  uint8_t tens = value / 10;
  uint8_t ones = value % 10;

  if (value < 10) {
    sendToMax7219(1, 0x0F);
  } else {
    sendToMax7219(1, tens);
  }

  sendToMax7219(2, ones);
}

void setup() {
  pinMode(BUT1, INPUT_PULLUP);
  pinMode(BUT2, INPUT_PULLUP);
  pinMode(BUT3, INPUT_PULLUP);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  initDisplay();
  showStartupTest();
  displayCount(count);
}

void loop() {
  unsigned long now = millis();

  int but1Reading = digitalRead(BUT1);
  int but2Reading = digitalRead(BUT2);
  int but3Reading = digitalRead(BUT3);

  // LED1 and LED2 follow button presses
  digitalWrite(LED1, but1Reading == LOW);
  digitalWrite(LED2, but2Reading == LOW);

  // --------------------
  // BUT3 hold-to-reset
  // --------------------
  if (but3Reading == LOW) {
    if (but3PressStart == 0) {
      but3PressStart = now;
      resetPerformed = false;
    }

    if ((now - but3PressStart) >= 800) {
      digitalWrite(LED3, HIGH);

      if (!resetPerformed) {
        count = 0;
        displayCount(count);
        resetPerformed = true;
      }
    } else {
      digitalWrite(LED3, LOW);
    }
  } else {
    but3PressStart = 0;
    resetPerformed = false;
    digitalWrite(LED3, LOW);
  }

  // --------------------
  // Increment (BUT1)
  // --------------------
  if (but1Reading != lastIncReading) {
    lastIncDebounceTime = now;
  }

  if ((now - lastIncDebounceTime) > debounceDelay) {
    if (but1Reading != incState) {
      incState = but1Reading;

      if (incState == LOW) {
        count = (count + 1) % 100;
        displayCount(count);
        lastBut1Time = now;
      }
    }
  }

  lastIncReading = but1Reading;

  // --------------------
  // Decrement (BUT2)
  // --------------------
  if (but2Reading != lastDecReading) {
    lastDecDebounceTime = now;
  }

  if ((now - lastDecDebounceTime) > debounceDelay) {
    if (but2Reading != decState) {
      decState = but2Reading;

      if (decState == LOW) {
        if (count > 0) {
          count--;
        } else {
          count = 99;
        }

        displayCount(count);
        lastBut2Time = now;
      }
    }
  }

  lastDecReading = but2Reading;

  // --------------------
  // LED2 bonus indicator
  // --------------------
  unsigned long diff =
      (lastBut1Time > lastBut2Time)
          ? (lastBut1Time - lastBut2Time)
          : (lastBut2Time - lastBut1Time);

  if (lastBut1Time != 0 &&
      lastBut2Time != 0 &&
      diff <= 2000) {
    digitalWrite(LED2, HIGH);
  }
}