#ifndef FLAKE_SNAKE_H
#define FLAKE_SNAKE_H

#include "Arduino.h"
#include "canvas.h"


class FLAKE_SNAKE {
  public:
    FLAKE_SNAKE(CANVAS *canvas):
      canvas(*canvas) {}
    void update(byte &frame);
    void restart();
    void handleTouch();

  private:
    CANVAS &canvas;
  
    byte mode;
    byte effectCountDown = 0;

    byte snakeAt = 0; // pin at which snake currently has head
    byte snakeLength = 2;
    byte fruitAt = random(0, 6); // pin where the fruit is

    void moveSnake();
    void repaint();
    void eatenEffect(byte frame);
    void lostEffect(byte frame);
    void wonEffect(byte frame);    
    byte nextLed(byte ledPin);
    byte prevLed(byte ledPin);
};

#endif
