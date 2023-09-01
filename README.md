# ArduinoAtomicClock
A basic atomic clock using an Arduino and a Rubidium Frequency Standard

This is my first Arduino project and it is almost useless without a Rubidium Frequency Standard with a 1 pulse per second output.
It also requires 2 rows of 8 digits segment display, a 4x4 keypad matrix and a knob with 9 positions connected with a resistor network array.

Rb frequency standard aren't that hard to find on the used market, at a surprisingly affordable price.
The segment displays, the keypad matrix, knob and resistor network array can be found of the shelf.
The frequency standard requires a 15V power supply capable of delivering 3A.

The project uses 3 libraries:
Arduino-Timer, for delaying function calls (https://www.arduino.cc/reference/en/libraries/arduino-timer/)
LedControl, for the segment displays (https://github.com/wayoda/LedControl)
Keypad (https://www.arduino.cc/reference/en/libraries/keypad/)

I used a rugged military enclosure from a 1980's era Magnetic Navigation Unit used in tanks.
For input, the keypad matrix uses pin 5 to 12, the knob uses analog pin A1 and the frequency standard send the 1PPS signal to pin 2 as well as a lock indication to analog pin A0.
For output, the segment displays uses pin 3, 4 and 13, as well a 2 individuals LEDs on analog pins A3 and A4 (that same signal is mirrored on analog pins A5 and A6) for the 1PPS and Lock signal respectively
(the 2 power indicator LEDs for 15V and 5V are pluged on the power rails).

The units provides a clock with date and time, a countdown and a chronometer.
The knob controls the mode, starting from the lower left clockwise, those are:
1-Diming of the segment display from 0 to 15, where the arrows (both L-R and U-D) cycles the levels and the numbers 0-9 set the value, E for max (15) and C for off/blackout.
2-Chronometer, where E start/stop the chrono and C reset it, the U-D arrows cycles the display format. (I will implement a lap functionality with the number keys laters)
3-Timer, where E start/stop the timer and C reset it, the U-D arrows cycles the display format.
4-Time, where the U-D arrows cycles the display time format, the differents formats are individualy available with the number keys.
5-Date, where the U-D arrows cycles the display date format, the differents formats are individualy available with the number keys.
6-Update date, the L-R keys move the cursor, the U-D increment/decrement the value at the cursor and the number keys set the value for the date in format YYYY.MM.DD.
7-Update time, the L-R keys move the cursor, the U-D increment/decrement the value at the cursor and the number keys set the value for the hours and minutes in 24H format, C is for clearing the second to zero, E is for incrementing the second by 10.
8-Update Timer, the L-R keys move the cursor, the U-D increment/decrement the value at the cursor and the number keys set the value for the timer in format HHH.MM.SSS. (values above 60 seconds are supproted)
9-Test, where all the segments and LEDs are turned on to test the display.
