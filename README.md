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

The second option is to manufacture the PCB and get all necessary parts yourself. Arduinoflake is Open-Source. [Schematic](doc/schematic.png) and [Gerber files](gerber/) are included in this repository. Don't forget to share your creation with me, I would love to see that!

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

Arduinoflake consists of 30 white LEDs grouped into 18 segments. Each group is connected to one of the ATmega8 output pins and can be programmatically controlled.  The touch button is handled by standalone TTP223 integrated circuit and is connected to PD3 input pin with interrupt support.

<p align="center">
  <img src="https://raw.githubusercontent.com/jpraus/arduinoflake/master/doc/pin-mapping.png" /><br />
  <em>Arduinoflake LED groups to ATmega8 digital pins mapping</em>
</p>

## Preparing your environment

Arduinoflake can be programmed using Arduino IDE. In order to do so, you need to have a programmer - special hardware to upload code to microchip - and library installed in Arduino IDE.

### USBasp

[USBasp](https://www.fischl.de/usbasp/) is a cheap $1 board that allows you to connect any ATmega chip with your computer via USB. Including Arduinoflake. It translates USB commands into AVR ISCP commands that can program the processor.

There is an ISCP interface in the left corner on the back of Arduinoflake. You can either use pogo pins if you have those or solder 6-pin header directly to the board to easily connect your ISCP cable. Before powering it up don't forget to change power on USBasp board to 3.3V.

<p align="center">
  <img src="https://raw.githubusercontent.com/jpraus/arduinoflake/master/doc/iscp.png" /><br />
  <em>AVR ISCP programming interface</em>
</p>

### Arduino as ISP (alternative to USBasp)

You can turn common Arduino UNO or any other Arduino board into programming hardware. its a bit messy, but does not require special hardware. [Read](https://www.arduino.cc/en/tutorial/arduinoISP) how to turn your Arduino UNO into a programmer. The rest is same as with USBasp.

### Installing MiniCore

It's time to start your Arduino IDE. In order to be able to program Arduinoflake, you need to first add support for ATmega8 device into Arduino IDE and configure it. 

- Install [MCUdude/MiniCore](https://github.com/MCUdude/MiniCore#how-to-install) device library 
- Open **Tools > Boards** and select **ATmega8**
- Select **Tools > Bootloader > No**
- Select **Tools > Clock > 8 MHz internal**
- Select **Tools > BOD > Disabled**

![Arduino IDE ATmega8](https://raw.githubusercontent.com/jpraus/arduinoflake/master/doc/arduinoide-atmega8.png)

### First upload

Checkout this repository open [arduinoflake.ino](src/arduinoflake/arduinoflake.ino) project file and hit Sketch > Upload Using Programmer. You should get a sweet *'avrdude done. Thank you.'* message at the end of the console.

![Arduino IDE upload](https://raw.githubusercontent.com/jpraus/arduinoflake/master/doc/arduinoide-upload.png)

## Getting started with the Arduinoflake library

... robots are building this page
