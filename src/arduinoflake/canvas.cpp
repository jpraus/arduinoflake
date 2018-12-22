#include "canvas.h"

// TODO: move this somewhere else
byte ledPins[] = {
  8, 5, 2, 19, 16, 12, // inner
  7, 4, 0, 17, 14, 10, // middle
  6, 3, 1, 18, 15, 11  // outer
};

CANVAS::CANVAS(byte _numLedsAtOnce) {
  numLedsAtOnce = _numLedsAtOnce;
  for (int i = 0; i < LEDS_COUNT; i++) {
    ledState[i] = 0;
  }
}

void CANVAS::setup() {
  for (int i = 0; i < LEDS_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void CANVAS::set(byte index, byte value) {
  ledState[index] = value;
}

void CANVAS::setByRange(byte fromIndex, byte toIndex, byte value) {
  for (byte i = fromIndex; i < toIndex; i++) {
    ledState[i] = value;
  }
}

void CANVAS::clear() {
  for (int i = 0; i < LEDS_COUNT; i++) {
    ledState[i] = LED_OFF; 
  }
}

void CANVAS::render() {
  byte origFrame = ledFrame;
  byte ledFrameTmp = ledFrame;

  byte counter = numLedsAtOnce;
  do {
    if (ledState[ledFrameTmp] > 0) {
      digitalWrite(ledPins[ledFrameTmp], HIGH);
      counter--;
    }
    ledFrameTmp++;
    if (ledFrameTmp == 18) {
      ledFrameTmp = 0;
    }
  } while (ledFrameTmp != origFrame && counter > 0); // try to render up to counter

  delayMicroseconds(1024);

  counter = numLedsAtOnce;
  do {
    if (ledState[ledFrame] > 0) {
      digitalWrite(ledPins[ledFrame], LOW);
      counter--;
    }
    ledFrame++;
    if (ledFrame == 18) {
      ledFrame = 0;
    }
  } while (ledFrame != origFrame && counter > 0); // try to render up to counter
}

/*
void renderByPwm() {
  // 0 off
  // 1-127 PWM (from off to fully lit)
  // 128 fully lit
  // 129-255 PWM reversed (from fully lit to off)

  if (ledState[_ledFrame] == LED_ON) { // fully lit
    digitalWrite(ledPins[_ledFrame], HIGH);
    delayMicroseconds(1024);
    digitalWrite(ledPins[_ledFrame], LOW);
  }
  else { // pseudo PWM (dimmed)
    int state = ledState[_ledFrame];
    if (state > LED_ON) {
      state = 255 - state; // reversed to 1-128
    }
    state = state << 3; // mapped to 0 - 1024 us

    digitalWrite(ledPins[_ledFrame], HIGH);
    delayMicroseconds(state);
    digitalWrite(ledPins[_ledFrame], LOW);
    delayMicroseconds(1024 - state);
  }

  _ledFrame += 1;
  if (_ledFrame >= 18) {
    _ledFrame = 0;
  }
}
*/
