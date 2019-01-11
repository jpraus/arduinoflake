#include "canvas.h"
#include "flakeSnake.h"
#include <CapacitiveSensor.h>

// 1 for CR2032, 5 for 2xAAA
#define BATTERY_LIFETIME 2 // from 1 (~11mA) to 18 (~80mA); 1 - longest lifetime, 18 - shortest lifetime

#define MODE_ANIMATIONS 0
#define MODE_SNAKE 1

#define TOUCH_PIN_SEND 13
#define TOUCH_PIN_RECEIVE 9
#define TOUCH_THRESHOLD 8
#define TOUCH_SENSITIVITY 5 // 3 short, 20 long

CANVAS snowflake(BATTERY_LIFETIME);
CapacitiveSensor touchButton = CapacitiveSensor(TOUCH_PIN_SEND, TOUCH_PIN_RECEIVE);
FLAKE_SNAKE flakeSnake(&snowflake);

byte mode = MODE_ANIMATIONS;
bool animationChanged = true;
byte animation = 0;
byte frame = 0;

boolean touched = false;
boolean touchHandled = false;
boolean touchStart = false;
byte touchCounter = 0;
byte touchSensitivity = TOUCH_SENSITIVITY;
int touchStrength = 0;

void setup() {
  randomSeed(analogRead(A7));
  
  snowflake.setup();
  flakeSnake.restart();
  touchButton.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn of auto-callibration

  animation = random(0, 6);
  mode = MODE_ANIMATIONS;
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

  // touch button recognition
  long touchValue = touchButton.capacitiveSensor(1);
  if (touchValue > TOUCH_THRESHOLD) { // touched
    if (touchCounter > TOUCH_SENSITIVITY) { // touched - prevention from random anomally
      touchStrength ++;
      touchStart = true;
    }
    else {
      touchCounter ++;
    }
  }
  else if (touchCounter > 0) {
    touchCounter --;
  }
  else if (touchStart) {
    touchStart = false;
    touched = true;
  }
  else {
    touched = false;
    touchHandled = false;
  }

  // long touch to change to snake
  if (mode == MODE_ANIMATIONS && touchStrength > 1000) {
    frame = 0;
    mode = MODE_SNAKE;
    flakeSnake.restart();
    touchHandled = true;
  }
  if (mode == MODE_SNAKE && touchStart) {
    touched = true; // touched on touch down in game mode
  }

  // touch handler
  if (touched && !touchHandled) {
    if (mode == MODE_ANIMATIONS) {
      frame = 0;
      animation ++;
      animationChanged = true;
      snowflake.clear();
    }
    else if (mode == MODE_SNAKE) {
      flakeSnake.handleTouch(); 
    }
    touchHandled = true;
    touchStrength = 0;
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

  if (frame < 80) { // speed of animation
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
