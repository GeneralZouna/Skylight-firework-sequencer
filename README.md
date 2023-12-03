# Aditional Comments on code:

## Configuration

### #define DEBUG
Comment or uncomment this option to enables serial debugging messages.

Reccomended to disable (comment) this option when not trubleshooting or testing.

### Definition of pins 

The variables that you should change are:

```cpp
#define DATA 11
#define CLK 12
#define LATCH 8
#define R_PIN 9
```
DATA pin is connected to the pin 14 on your 74HC595 chip, and is responsible for the data transfer.

CLK pin is connected to the pin 11 on your 74HC595 chip, and is responsible for giving a clock.

LATCH pin is connected to the pin 12 on your 74HC595 chip, and is responsible for moving the data form the internal inputs to the outputs of the shift registers.

R_PIN pin is connected to the pin 10 on your 74HC595 chip. R_PIN is for reset pin.

```
DISCLAMER:
R_PIN is not used if FLIPPED_OUTPUTS is enabled
```


To configure the NodeMcu borads for your configuration of wireing this are the pin numbers defined in the source code:
```cpp
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
```

Example:
If you have your 74HC595 register connected to pins D0 - data, D1 - clock, D2 - latch and D3 - rest, you would correct code to:
```cpp
#define DATA 16
#define CLK 5
#define LATCH 4
#define R_PIN 0
```

> **Tip:** If NodeMcu is crashing, try changing pins to different values


### #define FLIPPED_OUTPUTS

Comment this option if your switching borads are using active HIGH signal.
If it's uncommented all of the inputs will be inverted.

**Reccomended:** If poddible have this option commented out.

### #define FLIPPED_PORTS

Comment out this setting if your boards haven't been connected by bafoons.

It affects how data is stored in each shift register. Shift registers have 8 output pins, this defines if first pin is MSB or LSB.

If it's commented out it means it shifts out LSB first otherwise its MSB first.

> If you have output on shift register 1100 0000. There are two ways you can say which is for pin 1. Either leftmost bit is pin 1 or rightmost bit is pin 1, which will result in wildly different behaviours.

This setting applies to all boards


### #define MAX_ON_PINS

This defines maximum number of timers that can be running simultainiously. Which also effect maximum number of pins that can be turned on at one point.


### ON_TIME

It defines the lenght of time that output will be on for.


### #define OUTPUTS 

Defines number of outputs. This allows code to run more consistantly and faster. If you have problems make number devisable by 8. If you still have problerms increase the number to be 8 more than the number of outputs you have.

### program_step

This is an array of program steps. It contains array pair of 2 numbers:
1st int is pin and the 2nd int is time to wait for next step. the last pair must be {0,0} or the program will produce undefined behaviour.

Syntax:
```cpp
int program_steps[][2] = {
	{pin1,time_ms}, //Step 1
	{pin2,time_ms}, //step 2
		.
		.
		.
	{0,0} //last one aka. ending
}
```

## Setup of arduino IDE for NodeMcu

  1. Go to File>Preferences and add the following link to Aditional Boards Manager URLs: http://arduino.esp8266.com/stable/package_esp8266com_index.json
  2. Tools>Boards>Boards manager and search for ESP8266 by ESP8266 Community and click install
  3. Tools>Boards>ESP8266 boards>NodeMcu 1.0(ESP-12E Module)
  4. Configure the code.
  5. Upload

# Use instructions

  When you power on the NodeMcu it will create a wifi access point called "Firework Control". 
  
  Once conneted, go to site 192.168.4.1.
  
  There you have a red/green button to start and stop the lunch sequence
  
  
