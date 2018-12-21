#include <CapacitiveSensor.h>

#define BATTERY_LIFETIME 3 // from 1 (~11mA) to 18 (~80mA); 1 - longest lifetime, 18 - shortest lifetime

byte ledPins[] = {
  5, 2, 19, 16, 12, 8, // inner
  7, 4, 0, 17, 14, 10, // middle
  6, 3, 1, 18, 15, 11  // outer
};

#define LED_ON 128
#define LED_OFF 0
#define LEDS_COUNT 18

byte ledState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // inner, middle, edge

#define TOUCH_PIN_SEND 13
#define TOUCH_PIN_RECEIVE 9
#define TOUCH_THRESHOLD 8

#define TOUCH_SENSITIVITY_LONG 20
#define TOUCH_SENSITIVITY_SHORT 3

CapacitiveSensor touchButton = CapacitiveSensor(TOUCH_PIN_SEND, TOUCH_PIN_RECEIVE);

void setup() {
  touchButton.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn of auto-callibration

  for (int i = 0; i < 18; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

bool animationChanged = true;
byte animation = 0;
byte frame = 0;

boolean touched = false;
boolean touchHandled = false;
byte touchCounter = 0;
byte touchSensitivity = TOUCH_SENSITIVITY_LONG;

void loop() {
  switch (animation) {
    case 0:
      randomAnimation();
      break;
    case 1:
      glowAnimation();
      break;
    case 2:
      loopAnimation();
      break;
    case 3:
      snakeAnimation();
      break;
    case 4:
      fadingAnimation();
      break;
    default:
      animation = 0;
      break;
  }
  _render();
  frame ++;

  // touch button recognition
  long touchValue = touchButton.capacitiveSensor(1);
  if (touchValue > TOUCH_THRESHOLD) { // touched
    if (touchCounter > touchSensitivity) { // touched - prevention from random anomally
      touched = true;
    }
    else {
      touchCounter ++;
    }
  }
  else if (touchCounter > 0) {
    touchCounter --;
  }
  else {
    touched = false;
    touchHandled = false;
  }

  // touch handler
  if (touched && !touchHandled) {
    _clearAll();
    frame = 0;
    animation ++;
    animationChanged = true;
    touchHandled = true;
  }
}

void glowAnimation() {
  if (animationChanged) {
    for (int i = 0; i < 18; i ++) {
      ledState[i] = LED_ON;
    }
    animationChanged = false;
  }
}

/**
 * Circles animation - circles from center to the edges
 */
bool fadingAnimation() {
  if (animationChanged) {
    for (int i = 0; i < 6; i ++) {
      ledState[i] = 128;
    }
    for (int i = 6; i < 12; i ++) {
      ledState[i] = 64;
    }
    for (int i = 12; i < 18; i ++) {
      ledState[i] = 0;
    }
    animationChanged = false;
  }

  if (frame < 10) { // speed of animation
    return false;
  }
  frame = 0;

  for (int i = 0; i < 18; i ++) {
    ledState[i]++;
  }

  return true;
}

/**
 * Snake animation - 3 pixels snake running around
 */
byte snakeState = 0;

bool snakeAnimation() {
  if (animationChanged) {
    snakeState = 0;
    animationChanged = false;
  }
  
  if (frame < 150) { // speed of animation
    return false;
  }
  frame = 0;

  for (int i = 0; i < 18; i ++) { 
    ledState[i] = (i >= snakeState && i < snakeState + 3) ? LED_ON : LED_OFF;
  }
  if (snakeState >= 16) {
    for (int i = 6; i < 12; i ++) { 
      ledState[i] = (i >= snakeState - 12 && i < snakeState - 9) ? LED_ON : LED_OFF;
    }
  }
  if (snakeState >= 22 && snakeState < 25) {
    ledState[5] = LED_ON;
  }
  if (snakeState >= 23) {
    ledState[0] = LED_ON;
  }
  if (snakeState >= 24) {
    ledState[1] = LED_ON;
  }
  if (snakeState >= 25) {
    ledState[2] = LED_ON;
    ledState[5] = LED_OFF;
  }

  snakeState += 1;
  if (snakeState >= 25) {
    snakeState = 0;
    return true; // finsihed
  }

  return false;
}

/**
 * Loop animation - circling on and off
 */
byte loopState = 0;

bool loopAnimation() {
  if (animationChanged) {
    loopState = 0;
    animationChanged = false;
  }
  
  if (frame < 150) { // speed of animation
    return false;
  }
  frame = 0;

  if (loopState < 6) {
    for (int i = loopState; i < 18; i += 6) {
      ledState[i] = LED_ON;
    }
  }
  else {
    for (int i = loopState - 6; i < 18; i += 6) {
      ledState[i] = LED_OFF;
    }
  }

  loopState += 1;
  if (loopState >= 12) {
    loopState = 0;
    return true; // finsihed
  }

  return false;
}

/**
 * Random animation - one random LED at the time
 */
byte lastRandom = 0;

void randomAnimation() {
  if (animationChanged) {
    lastRandom = 0;
    animationChanged = false;
  }

  if (frame < 10) { // speed of animation
    return;
  }
  frame = 0;

  ledState[lastRandom] = LED_OFF;
  lastRandom = random(0, 18);
  ledState[lastRandom] = LED_ON;
}

/**
 * Render functions
 */
byte _ledFrame = 0;

void _render() {
  byte origFrame = _ledFrame;
  byte ledFrame = _ledFrame;

  byte counter = BATTERY_LIFETIME;
  do {
    if (ledState[ledFrame] > 0) {
      digitalWrite(ledPins[ledFrame], HIGH);
      counter--;
    }
    ledFrame++;
    if (ledFrame == 18) {
      ledFrame = 0;
    }
  } while (ledFrame != origFrame && counter > 0); // try to render up to counter

  delayMicroseconds(1024);

  counter = BATTERY_LIFETIME;
  do {
    if (ledState[_ledFrame] > 0) {
      digitalWrite(ledPins[_ledFrame], LOW);
      counter--;
    }
    _ledFrame++;
    if (_ledFrame == 18) {
      _ledFrame = 0;
    }
  } while (_ledFrame != origFrame && counter > 0); // try to render up to counter

  // 0 off
  // 1-127 PWM (from off to fully lit)
  // 128 fully lit
  // 129-255 PWM reversed (from fully lit to off)

/*
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
*/
}

void _clearAll() {
  for (int i = 0; i < 18; i++) {
    ledState[i] = LED_OFF; 
  }
}
