#include "flakeSnake.h"

#define MODE_RUN 0
#define MODE_EATEN 1
#define MODE_LOST 2
#define MODE_WON 3

void FLAKE_SNAKE::update(byte &frame) {
  // effect
  if (effectCountDown > 0) {
    if (mode == MODE_EATEN) {
      eatenEffect(frame);
    }
    else if (mode == MODE_WON) {
      wonEffect(frame);
    }
    else if (mode == MODE_LOST) {
      lostEffect(frame);
    }
  }

  // gameplay
  else if (frame >= 250) { // speed of gameplay
    frame = 0;
    moveSnake();    
  }
}

void FLAKE_SNAKE::restart() {
  snakeAt = 0;
  snakeLength = 2;
  fruitAt = random(0, 6);
  mode = MODE_RUN;
}

void FLAKE_SNAKE::moveSnake() {
  if (mode == MODE_EATEN && snakeLength == 5) {
    effectCountDown = 100;
    mode = MODE_WON;
  }
  else if (mode == MODE_EATEN) {
    snakeLength ++;
    snakeAt = nextLed(snakeAt);
    fruitAt = random(0, 6);
    mode = MODE_RUN;
  }
  else if (mode == MODE_LOST || mode == MODE_WON) {
    restart();
  }
  else if (mode == MODE_RUN) {
    snakeAt = nextLed(snakeAt);
  }

  repaint();
}

void FLAKE_SNAKE::handleTouch() {
  if (mode == MODE_RUN) {
    if (fruitAt == snakeAt) {
      mode = MODE_EATEN;
      effectCountDown = 25;
      repaint();
    }
    else {
      mode = MODE_LOST;
      effectCountDown = 25;
      repaint();
    }
  }
}

void FLAKE_SNAKE::repaint() {
  canvas.clear();

  // render snake
  byte ledPin = snakeAt;
  for (int i = 0; i < snakeLength; i++) {
    canvas.set(ledPin, LED_ON);
    ledPin = prevLed(ledPin);
  }

  // render mouth
  if (mode == MODE_EATEN || mode == MODE_LOST) {
    canvas.set(snakeAt + 6, LED_ON);
  }

  // render fruit
  canvas.set(fruitAt + 12, LED_ON);
}

void FLAKE_SNAKE::eatenEffect(byte &frame) {
  if (frame >= 50) { // speed of effect
    frame = 0; 
    effectCountDown --;

    byte state = (effectCountDown % 2 == 0) ? LED_OFF : LED_ON;
    canvas.setByRange(12, 18, state);
  }
}

void FLAKE_SNAKE::lostEffect(byte &frame) {
  if (frame >= 100) { // speed of effect
    frame = 0; 
    effectCountDown --;

    byte state = (effectCountDown % 2 == 0) ? LED_OFF : LED_ON;
    byte ledPin = snakeAt;
  
    for (int i = 0; i < snakeLength; i++) {
      canvas.set(ledPin, state); // body
      ledPin = prevLed(ledPin);
    }
    canvas.set(snakeAt + 6, state); // mouth
  }
}

void FLAKE_SNAKE::wonEffect(byte &frame) {
  if (frame >= 50) { // speed of effect
    frame = 0; 
    effectCountDown --;

    byte state = (effectCountDown % 2 == 0) ? LED_OFF : LED_ON;
    canvas.setByRange(0, 6, state);
  
    state = (effectCountDown % 2 == 0) ? LED_ON : LED_OFF;
    canvas.setByRange(12, 18, state);
  }
}

byte FLAKE_SNAKE::nextLed(byte ledPin) {
  if (ledPin == 5) {
    return 0;
  }
  return ledPin + 1;
}

byte FLAKE_SNAKE::prevLed(byte ledPin) {
  if (ledPin == 0) {
    return 5;
  }
  return ledPin - 1;
}
