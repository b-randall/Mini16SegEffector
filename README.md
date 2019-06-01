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
- 1 x 1k resistor
- 1 x TLC5940 16 channel led display driver
- 1 x 74HC595 8-bit bit shifter


