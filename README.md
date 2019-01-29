# Arduinoflake

Arduinoflake is an awesome winter decoration that will light up every dark night. It is no ordinary decoration. This frozen looking beauty is Arduino compatible and can be programmed to do whatever you want to! It features a capacitive touch button which let you interact with it. Last but not least it is **open-source**.

![Arduinoflakes](https://raw.githubusercontent.com/jpraus/arduinoflake/master/doc/PC172551.JPG)

Sponsored by [PCBWay](https://www.pcbway.com/) - PCB prototype the easy way.

## Features

- 30 LEDs grouped into 18 independent segments
- touch button for interactions
- ATmega8 Arduino compatible microprocessor
- AVR ISCP programming interface
- runs up to 12 hours on a single CR2032 battery

## Building your Arduinoflake

The easiest way how to get your own snowflake is to order a DIY kit on my [Tindie Store](https://www.tindie.com/products/jiripraus/arduinoflake/). Kit contains professionally manufactured PCBs by PCBway and all compatible components needed to assemble a fully functional Arduinoflake.

The second option is to manufacture the PCB and get all necessary parts yourself. Arduinoflake is Open-Source. Schema and Gerber files are included in this repository. Don't forget to share your creation with me, I would love to see that!

### Parts list

- ATmega8A TQF32
- TTP223 SOT23
- 18x bright white flat-top crystal clear LEDs
- 12x bright white 1206 SMD LEDs
- 18x 68Ω resistors
- 3x 100nF capacitors
- 10kΩ resistor
- CR2032 battery holder
- MSK-12C02 power switch

## Understanding Arduinoflake hardware

Arduinoflake consists of 30 white LEDs grouped into 18 segments. Each segment is connected to one of the ATmega8 output pins and can be programmatically controlled.  The touch button is handled by standalone TTP223 integrated circuit and is connected to PD3 input pin with interrupt support.
