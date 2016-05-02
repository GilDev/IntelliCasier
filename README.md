IntelliCasier
=============

This project uses a standard school locker with two 8*8 holes grids. 5 mm LEDs fits beautifully in those holes, small tactile switches too, a LCD display can be added in the name's tag space, thus a boring locker can be easily transformed into a multi-game arcade cabinet. Enjoy!

*Prototype:*
![Second prototype](assets/photos/SecondPrototype2.jpg)

[Presentation video in french](https://www.youtube.com/watch?v=VD672DgCmok), shows the running prototype.

This project is licensed under the terms of the MIT license. Check [`LICENSE.txt`](LICENSE.txt) for more informations.


Features
--------

* Main menu (LCD + Matrix):
  * 1 player games:
    * Snake
	* Flappy Bird
    * Breakout **(not currently implemented)**
    * Race **(not currently implemented)**
  * 2 player games:
    * Pong
    * Snake
  * About screen with hidden option menu & student's schedule display
* Event-driven software
* Multi-languages interface, easily localizable
* Screensaver with delay depending on context
* High-scores logging **(not currently implemented)**
* Sounds **(not currently implemented)**
* Alarm when opening locker without pressing a predefined sequence of buttons **(not currently implemented)**
* Automatic standby based on time **(not currently implemented)**


Requirements
------------

* [Arduino (I use a Nano with an ATmega328)](http://www.banggood.com/ATmega328P-Nano-V3-Controller-Board-Compatible-Arduino-p-940937.html)
* [MAX7219/MAX7221 with 8*8 LED matrix](http://www.banggood.com/MAX7219-Dot-Matrix-MCU-LED-Display-Control-Module-Kit-For-Arduino-p-915478.html)
* [I2C 16*8 LCD display](http://www.banggood.com/IIC-I2C-1602-Blue-Backlight-LCD-Display-Module-For-Arduino-p-950726.html)
* 5 momentary pushbuttons
* [DS3231 I2C RTC](http://www.banggood.com/DS3231-AT24C32-IIC-High-Precision-Real-Time-Clock-Module-For-Arduino-p-81066.html) *(optional)* **(not currently implemented)**
* Piezoelectric buzzer *(optional)* **(not currently implemented)**
* Photocell (LDR) *(optional)* **(not currently implemented)**

Please look at [`config.h`](src/config.h) to configure the system and pins.


Compilation
-----------

I currently use [Arturo](https://github.com/scottdarch/Arturo) to compile the sketch. If you want to use the Arduino IDE, you must:

* Copy all libraries in [`lib`](lib) in Arduino's `libraries` folder or use the Library Manager to install them (Arduino IDE >= 1.6.2))
* Add `#include <LedControl.h>`, `#include <LiquidCrystal_I2C.h>` and `#include <Sodaq_3231>` at the top of [`src/ArcadeLocker.ino`](src/ArcadeLocker.ino)
* Rename the [`src`](src) directory to `ArcadeLocker`


Todo
----

* Better localization system (kind of ugly right now, I guess?)
* Events handling optimization (loops)
* Enhance `newLcdScroll()` (start position, spaces before looping)
* Settings storage in EEPROM
* High scores with the ability to enter a name or initials
* **Maybe add a custom `malloc()` to dynamically allocate app's variable memory and not having all those global variables taking place in memory (or just use `malloc()`?)**
* **Put animations instead of scrolling text on matrix and maybe remove all that code to scroll text on the matrix now that there is an LCD display**
* Remove unnecessary timer events when first launching them in games (just call the callback function, no need to wait on start)
* Reorganize functions order in apps' code to stay consistent between them
* Enhance `printLcd()` to center text automatically? (would consume cycles using `strlen()`)
* Put strings buffers in an array?
* Add animations to screensaver