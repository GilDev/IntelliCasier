Arcade Locker
=============

This project uses a standard school locker with two 8*8 holes grids. 5 mm LEDs fits beautifully in those holes, small tactile switches too, thus a boring locker can be easily transformed into a multi-game arcade cabinet. Enjoy!

This project is licensed under the terms of the MIT license. Check [`LICENSE.md`](LICENSE.md) for more informations.

Requirements
------------

* Arduino (I use a Nano with an ATmega328)
* MAX7219/MAX7221
* 8*8 LED matrix
* 5 buttons


Compilation
-----------

I actually use [Arturo](https://github.com/scottdarch/Arturo) to compile the sketck. If use the Arduino IDE, you will need to:

* Install the LedControl library (copy [`lib/LedControl`](lib/LedControl) directory in your `libraries` folder or use the Library Manager (Arduino >= 1.6.2))
* Add `#include <LedControl.h>` at the top of [`src/ArcadeLocker.ino`](src/ArcadeLocker.ino)
* Put all files in `src` in a directory named `ArcadeLocker`
