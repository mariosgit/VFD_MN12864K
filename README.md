
# AdafruitGFX lib compatible driver for an MN12832JC VFD display.

## Experimental !!!

Supposed to work with TeensyLC, could be possible on Arduinos too, just check the refreshDisplay interrupt call.

Todo
* GLAT is high in the connection drawing ???
* GBLK ???
* resistor at VDD2

### Dependencies:
- mbLog, stdout style serial log, can be found in mariosgit/mbLog

### Pinout, Connections
- These are hardcoded, see mbGFX_MN12864K.cpp

| name | stm32 | mn12864k |
|---|---|---|
| SCLK    | PA5 | 8 & 13  |
| MOSI    | PA7 | 9       |
| pinBLK1 | PB1 | 6 = 1st of the many pins |
| pinLAT1 | PB0 | 7 |
| pinGBLK | PA2 | 15 ??? what for ??? |
| pinGLAT | PB0 | 14 - connect to LAT1 PB0 |
| pinGCLK | PA5 | 13 - connect to SCLK PA5 |
| pinGSIN | --- | 12 - connect to SOUT pin 10 |

