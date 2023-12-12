# Additional Comments on Code:

## Configuration

### #define DEBUG

Comment or uncomment this option to enable serial debugging messages. 

Recommended to disable (comment) this option when not troubleshooting or testing.

### Definition of Pins

The variables that you should change are:

```cpp
#define DATA 11
#define CLK 12
#define LATCH 8
#define R_PIN 9
```

The DATA pin is connected to pin 14 on your 74HC595 chip and is responsible for data transfer.

The CLK pin is connected to pin 11 on your 74HC595 chip and is responsible for providing a clock.

The LATCH pin is connected to pin 12 on your 74HC595 chip and is responsible for moving the data from the internal inputs to the outputs of the shift registers.

The R_PIN pin is connected to pin 10 on your 74HC595 chip and is used for the reset pin.

```
DISCLAIMER:
R_PIN is not used if FLIPPED_OUTPUTS is enabled
```

To configure the NodeMcu boards for your wiring configuration, these are the pin numbers defined in the source code:

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

Example: If you have your 74HC595 register connected to pins D0 - data, D1 - clock, D2 - latch, and D3 - reset, you would correct the code to:

```cpp
#define DATA 16
#define CLK 5
#define LATCH 4
#define R_PIN 0
```

> **Tip:** If the NodeMcu is crashing, try changing pins to different values.

### #define FLIPPED_OUTPUTS

Comment this option if your switching boards are using an active HIGH signal. If it's uncommented, all inputs will be inverted.

**Recommended:** If possible, have this option commented out.

### #define FLIPPED_PORTS

Comment out this setting if your boards haven't been connected by buffoons.

It affects how data is stored in each shift register. Shift registers have 8 output pins; this defines if the first pin is MSB or LSB.

If it's commented out, it means it shifts out LSB first; otherwise, it's MSB first.

> If you have output on shift register 1100 0000, there are two ways you can say which is for pin 1: Either the leftmost bit is pin 1 or the rightmost bit is pin 1, which will result in wildly different behaviors.

This setting applies to all boards.

### #define MAX_ON_PINS

This defines the maximum number of timers that can be running simultaneously. It also affects the maximum number of pins that can be turned on at one point.

### ON_TIME

It defines the length of time that the output will be on for.

### #define OUTPUTS 

Defines the number of outputs. This allows the code to run more consistently and faster. If you have problems, make the number divisible by 8. If you still have problems, increase the number to be 8 more than the number of outputs you have.

### program_step

This is an array of program steps. It contains an array pair of 2 numbers: 1st int is pin and the 2nd int is the time to wait for the next step. The last pair must be {0,0} or the program will produce undefined behavior.

Syntax:

```cpp
int program_steps[][2] = {
    {pin1, time_ms}, // Step 1
    {pin2, time_ms}, // Step 2
    .
    .
    .
    {0, 0} // Last one aka. ending
}
```

## Setup of Arduino IDE for NodeMcu

1. Go to File > Preferences and add the following link to Additional Boards Manager URLs: http://arduino.esp8266.com/stable/package_esp8266com_index.json
2. Tools > Boards > Boards Manager and search for ESP8266 by ESP8266 Community and click install
3. Tools > Boards > ESP8266 boards > NodeMcu 1.0 (ESP-12E Module)
4. Configure the code.
5. Upload

# Usage Instructions

When you power on the NodeMcu, it will create a Wi-Fi access point called "Firework Control."

Once connected, go to the site 192.168.4.1.

There you have a red/green button to start and stop the launch sequence.
```
