# Totobot
This is an [mBlock 3](http://www.mblock.cc/mblock-software/) extension.

## Pinout
* D4, D7, D8, D12 - Motor Shield (74HC595)
  * D11 - M1 (LEFT_MOTOR)
  * D13 - M2 (RIGHT_MOTOR)
  * D5 - M3
  * D6 - M4
* A0 - Face

## EEPROM

* 0x0200 - magic number (0x44)
* 0x0201 - data size (8)
* 0x0202, 0x0203 - operating range for M1 (LEFT_MOTOR)
* 0x0204, 0x0205 - operating range for M2 (RIGHT_MOTOR)
* 0x0206, 0x0207 - operating range for M3
* 0x0208, 0x0209 - operating range for M4

## Credits
The app uses:
* [FastLED](https://github.com/FastLED/FastLED) 3.2.9
* [Adafruit Motor Shield library](https://github.com/adafruit/Adafruit-Motor-Shield-library) 1.0.1
* [TimerOne](http://playground.arduino.cc/Code/Timer1) 1.1 ([github.com](https://github.com/PaulStoffregen/TimerOne))
* [Arduboy-TinyFont](https://github.com/yinkou/Arduboy-TinyFont)
