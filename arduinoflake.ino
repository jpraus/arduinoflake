#include <CapacitiveSensor.h>

#define TOUCH_PIN 9

CapacitiveSensor touchSensor = CapacitiveSensor(20,9);

byte ledPins[] = {
  5, 2, 19, 16, 12, 8, // inner
  7, 4, 0, 17, 14, 10, // middle
  6, 3, 1, 18, 15, 11  // outer
};

#define LED_ON 128
#define LED_OFF 0
#define LEDS_COUNT 18

byte ledState[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // inner, middle, edge

void setup() {
  pinMode(TOUCH_PIN, INPUT);
  digitalWrite(TOUCH_PIN, LOW);

  for (int i = 0; i < 18; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

bool animationChanged = true;
byte animation = 0;
byte frame = 0;

void loop() {
  switch (animation) {
    case 0:
      glowAnimation();
      break;
    case 1:
      circleAnimation();
      break;
    case 2:
      snakeAnimation();
      break;
    case 3:
      loopAnimation();
      break;
    case 4:
      randomAnimation();
      break;
    case 5: 
      //glowAnimation();
      break;
    default:
      animation = 0;
      break;
  }
  _render();
  frame++;
  animationChanged = false;
}

void glowAnimation() {
  if (animationChanged) {
    for (int i = 0; i < 18; i ++) {
      ledState[i] = LED_ON;
    }
  }
}

byte circleState = 0;

/**
 * Circles animation - circles from center to the edges
 */
bool circleAnimation() {
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
  if (frame < 10) { // speed of animation
    return;
  }
  frame = 0;

  ledState[lastRandom] = LED_OFF;
  lastRandom = random(0, 18);
  ledState[lastRandom] = LED_ON;
}

/**
 * Utility functions
 */
byte _ledFrame = 0;

void _render() {
  while (ledState[_ledFrame] == 0) {
    _ledFrame += 1;
    if (_ledFrame >= 18) {
      _ledFrame = 0;
      return; // nothing to render
    }
  }

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
