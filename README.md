# arduino-relay-control
Control relays using simple serial commands. Supports up to 10 relays.

## How to use
- Connect relays to digital output pins on whatever Arduino via appropriate isolation circuitry, starting at some pin (default 5), sequentially
- Set the start pin and number of relays at the top of the sketch
- Flash this sketch to the Arduino
- Connect to the Arduino's serial port with 9600 baud
- Issue commands like 'r31' to turn relay 3 on, 'r00' to turn relay 0 off. You can send line breaks etc but they will be ignored- "r" is the delimiter.

If the outputs are the wrong way around for your board, change invert to true in the sketch and upload it again.

There's also a couple of variants which support a DHT11 temp/humidity sensor, and DS18B20 temp sensors (plural), outputting the values to serial every minute by default.
