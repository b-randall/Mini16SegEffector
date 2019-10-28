# Mini16SegEffector
Mini 16-Segment and Effect Arduino Shield panel for LDJ

uses TLC5940 16 channel LED driver and 74HC595 8-bit bit shifter for driving 16 seg display


- Requires Arduino Uno R3 with [Hoodloader 2](https://github.com/NicoHood/HoodLoader2)
- Gamepad Implementation modified from [HID-Project](https://github.com/NicoHood/HID)


# ToDo: 
- create font for 16 Seg TLC
- add TLC communcation
- write vefxio.dll implemenation for certain LDJ tools 

# Parts list:
- 1 x Arduino Uno R3 (with 16u2 AVR), may work on mega, untested
- 9 x 16 segment common cathode LED displays
- 9 x NPN transistors/mosfets
- 5 x 10k slide potentiometers (official use ALPS 60cm(?) with center detent, hard to source similar spec)
- 4 x mechanical switches 
- 1 x 2k resistor
- 1 x 10k resistor
- other resistors probably
- 1 x TLC5940 16 channel led display driver
- 1 x 74HC595 8-bit bit shifter


# setup:
      TLC (from TLC example):
      -  +5V from Arduino -> TLC pin 21 and 19     (VCC and DCPRG)
      -  GND from Arduino -> TLC pin 22 and 27     (GND and VPRG)
      -  digital 3        -> TLC pin 18            (GSCLK)
      -  digital 9        -> TLC pin 24            (XLAT)
      -  digital 10       -> TLC pin 23            (BLANK)
      -  digital 11       -> TLC pin 26            (SIN)
      -  digital 13       -> TLC pin 25            (SCLK)
      
      74hc595:
      -  digital 4        -> 74hc pin 11          (SH_CP)
      -  digital 2        -> 74hc pin 14          (DATA)
      -  digital 7        -> 74hc pin 12          (ST_CP)
      -  digital 6        -> 74hc pin 13          (OE)
