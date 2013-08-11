For now, it does the following:

 * Intercepts right/left signals from the RC Car RX chip.
 * Sends signals to the Arduino.
 * Forwards them back to the RC Car's H-Bridge.
 * If one is activated two times fast, disable steering temporarily.

In the future, I'd like to hook it up to an IR receiver (so that it can be hit by a regular TV remote).

After that, place a transmitter as well, so that it can fire at other cars.
