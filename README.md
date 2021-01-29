# ArduinoConway

## Parts

* a green LED
* a red LED
* 2 100 Ohm resistors
* an Arduino Uno
* a 16x2 LCD
* a 10K potentiometer

## Connecting

### LCD

* VSS to Arduino GND
* VDD to Arduino 5V
* V0 to potentiometer
* RS to Arduino D12
* RW to Arduino Arduino GND
* E to Arduino D11
* D4 to Arduino D5
* D5 to Arduino D4
* D6 to Arduino D3
* D7 to Arduino D2
* A to Arduino 5V
* K to Arduino GND

### Potentiometer

* middle pin to LCD V0 for contrast
* a pin to Arduino 5V, the other to the Arduino GND

## LEDS

* green:
    * Arduino **D8** to resistor
    * other side of resistor to anode (longest leg)
    * cathode (smallest leg) to Arduino GND
* red:
    * Arduino **D9** to resistor
    * other side of resistor to anode (longest leg)
    * cathode (smallest leg) to Arduino GND