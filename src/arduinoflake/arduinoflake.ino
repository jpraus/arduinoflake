#include "canvas.h"
#include "flakeSnake.h"

// 2 for CR2032, 5 for 2xAAA
#define BATTERY_LIFETIME 2 // from 1 (~11mA) to 18 (~80mA); 1 - longest lifetime, 18 - shortest lifetime

#define MODE_ANIMATIONS 0


#define MODE_SNAKE 1

#define TOUCH_SENSOR_PIN 3
#define TOUCH_MODE_ON_UP 0 // recognize touch on touch up
#define TOUCH_MODE_ON_DOWN 1 // recognize touch on touch down

CANVAS snowflake(BATTERY_LIFETIME);
FLAKE_SNAKE flakeSnake(&snowflake);

byte mode = MODE_ANIMATIONS;
bool animationChanged = true;
byte animation = 0;
byte frame = 0;

byte touchMode = TOUCH_MODE_ON_UP;
unsigned long touchStartMs = 0;
unsigned long touchDuration = 0;
boolean touched = false;
boolean touchHandled = true;

void setup() {
  randomSeed(analogRead(A7));
  
  snowflake.setup();
  flakeSnake.restart();

  animation = random(0, 6);
  mode = MODE_ANIMATIONS;

  pinMode(TOUCH_SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(TOUCH_SENSOR_PIN), touchISR, CHANGE);
}

void loop() {
  if (mode == MODE_ANIMATIONS) {
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
        circleAnimation(true);
        break;
      case 4:
        fanAnimation();
        break;
      case 5:
        circleAnimation(false);
        break;
      default:
        animation = 0;
        break;
    }
  }
  else if (mode == MODE_SNAKE) {
    flakeSnake.update(frame);
  }

  snowflake.render();
  frame ++;

  // handle touch
  if (touched && !touchHandled) {
    if (mode == MODE_ANIMATIONS && touchDuration > 1000) { // long touch to change to snake
      frame = 0;
      mode = MODE_SNAKE;
      flakeSnake.restart();
      touchMode = TOUCH_MODE_ON_DOWN;
    }
    else if (mode == MODE_ANIMATIONS) {
      frame = 0;
      animation ++;
      animationChanged = true;
      snowflake.clear();
    }
    else if (mode == MODE_SNAKE) {
      flakeSnake.handleTouch(); 
    }

    touchHandled = true;
  }
}


void touchISR() {
  byte touchOn = digitalRead(TOUCH_SENSOR_PIN);
  if (touched && !touchHandled) {
    return; // previous touch was not handled yet.
  }
  if (touchOn == HIGH) {
    if (touchMode == TOUCH_MODE_ON_DOWN) {
      touched = true;
      touchHandled = false;
    }
    else if (touchMode == TOUCH_MODE_ON_UP) {
      touchStartMs = millis();
      touchDuration = 0;
    }
  }
  else if (touchStartMs > 0 && touchMode == TOUCH_MODE_ON_UP) {
    touchDuration = millis() - touchStartMs;
    touchStartMs = 0;
    touched = true;
    touchHandled = false;
  }
}

/**
 * Glow animation - fully lit snowflake
 */
void glowAnimation() {
  if (animationChanged) {
    snowflake.setByRange(0, 18, 128);
    animationChanged = false;
  }
}

/**
 * Circles animation - circles from center to the edges
 */
byte circleIndex = 0;

void circleAnimation(bool bounce) {
  if (animationChanged) {
    circleIndex = 0;
    animationChanged = false;
  }

  if (frame < 255) { // speed of animation
    return;
  }
  frame = 0;

  snowflake.clear();
  switch (circleIndex) {
    case 0:
      snowflake.setByRange(0, 6, LED_ON);
      break;
    case 1:
    case 3:
      snowflake.setByRange(6, 12, LED_ON);
      break;
    case 2:
      snowflake.setByRange(12, 18, LED_ON);
      break;
  }

  circleIndex ++;
  if (circleIndex == 3 && !bounce || circleIndex >= 4) {
    circleIndex = 0;
  }
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
    snowflake.set(i, (i >= snakeState && i < snakeState + 3) ? LED_ON : LED_OFF);
  }
  if (snakeState >= 16) {
    for (int i = 6; i < 12; i ++) { 
      snowflake.set(i, (i >= snakeState - 12 && i < snakeState - 9) ? LED_ON : LED_OFF);
    }
  }
  if (snakeState >= 22) {
    snowflake.set(0, LED_ON);
  }
  if (snakeState >= 23) {
    snowflake.set(1, LED_ON);
  }
  if (snakeState >= 24) {
    snowflake.set(2, LED_ON);
  }

  snakeState += 1;
  if (snakeState >= 24) {
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
      snowflake.set(i, LED_ON);
    }
  }
  else {
    for (int i = loopState - 6; i < 18; i += 6) {
      snowflake.set(i, LED_OFF);
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
 * Fan animation - spining rotor of a fan
 */
byte fanIndex = 0;

void fanAnimation() {
  if (animationChanged) {
    fanIndex = 0;
    animationChanged = false;
  }

  if (frame < 90) { // speed of animation
    return;
  }
  frame = 0;

  snowflake.clear();
  for (byte i = fanIndex; i < 18; i += 3) {
    snowflake.set(i, LED_ON);
  }

  fanIndex ++;
  if (fanIndex >= 3) {
    fanIndex = 0;
  }
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

  snowflake.set(lastRandom, LED_OFF);
  lastRandom = random(0, 18);
  snowflake.set(lastRandom, LED_ON);
}
