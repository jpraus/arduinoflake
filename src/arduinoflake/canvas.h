#ifndef CANVAS_H
#define CANVAS_H

#include "Arduino.h"

#define LED_ON 128
#define LED_OFF 0
#define LEDS_COUNT 18

class CANVAS {
  public:
    CANVAS(byte numLedsAtOnce);
    void setup();
    void render();

    void set(byte index, byte value);
    void setByRange(byte fromIndex, byte toIndex, byte value);
    void clear();

  private:
    byte numLedsAtOnce;
    byte ledState[LEDS_COUNT];
    byte ledFrame;
};

#endif
