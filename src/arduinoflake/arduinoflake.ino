#include "canvas.h"
#include "flakeSnake.h"
#include <CapacitiveSensor.h>

#define BATTERY_LIFETIME 3 // from 1 (~11mA) to 18 (~80mA); 1 - longest lifetime, 18 - shortest lifetime

#define TOUCH_PIN_SEND 13
#define TOUCH_PIN_RECEIVE 9
#define TOUCH_THRESHOLD 8

#define TOUCH_SENSITIVITY_LONG 20
#define TOUCH_SENSITIVITY_SHORT 3

CANVAS snowflake(BATTERY_LIFETIME);
CapacitiveSensor touchButton = CapacitiveSensor(TOUCH_PIN_SEND, TOUCH_PIN_RECEIVE);
FLAKE_SNAKE flakeSnake(&snowflake);

bool animationChanged = true;
byte animation = 0;
byte frame = 0;

boolean touched = false;
boolean touchHandled = false;
byte touchCounter = 0;
byte touchSensitivity = TOUCH_SENSITIVITY_LONG;

void setup() {
  snowflake.setup();
  flakeSnake.restart();
  touchButton.set_CS_AutocaL_Millis(0xFFFFFFFF); // turn of auto-callibration
}

void loop() {
  switch (animation) {
    case 0:
      flakeSnake.update(frame);
      //randomAnimation();
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
  snowflake.render();
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
    flakeSnake.handleTouch();
    
    //snowflake.clear();
    //frame = 0;
    //animation ++;
    //animationChanged = true;
    touchHandled = true;
  }
}

void glowAnimation() {
  if (animationChanged) {
    snowflake.setByRange(0, 18, 128);
    animationChanged = false;
  }
}

/**
 * Circles animation - circles from center to the edges
 */
bool fadingAnimation() {
  if (animationChanged) {
    snowflake.setByRange(0, 6, 128);
    snowflake.setByRange(6, 12, 64);
    snowflake.setByRange(12, 18, 0);
    animationChanged = false;
  }

  if (frame < 10) { // speed of animation
    return false;
  }
  frame = 0;

  for (int i = 0; i < 18; i ++) {
    //ledState[i]++;
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
    snowflake.set(i, (i >= snakeState && i < snakeState + 3) ? LED_ON : LED_OFF);
  }
  if (snakeState >= 16) {
    for (int i = 6; i < 12; i ++) { 
      snowflake.set(i, (i >= snakeState - 12 && i < snakeState - 9) ? LED_ON : LED_OFF);
    }
  }
  if (snakeState >= 22 && snakeState < 25) {
    snowflake.set(5, LED_ON);
  }
  if (snakeState >= 23) {
    snowflake.set(0, LED_ON);
  }
  if (snakeState >= 24) {
    snowflake.set(1, LED_ON);
  }
  if (snakeState >= 25) {
    snowflake.set(2, LED_ON);
    snowflake.set(5, LED_OFF);
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
