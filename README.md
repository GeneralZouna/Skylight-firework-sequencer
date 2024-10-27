# Skylight 
### Simple ESP32 based firework sequencer
---

This is my simple firework sequencer based on ESP32 microcontrollers.

# Setup
1) Assemble master node by connecting 2 ESP32 modules:
    pin 17 - pin 16
    pin 16 - pin 16
2) Flash Skylight program on one of them
2.1) Upload the files in Skylight/data folder onto the ESP32 with LittleFS
3) Flash Packet forwarder on second one
4) Flash SkylightSlave on the relay board
5) Connect to wifi "Skylight"
6) Open website 192.168.4.1
7) Have fun!

Check back to see for documentation.

For any help feel free to contact me

# Materials used
* ESP32Wroom32U Dev_kit (2 modules)
* 16 Channel Relay Module ESP8266 Development Board (Currently supporting up to 20 boards)

# Libraries and code used:
* [Picnic](https://github.com/franciscop/picnic) <- I edited the colors a bit to make it more of a dark mode
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer) <- Wow this one made my life easier
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [LittleFS](https://github.com/littlefs-project/littlefs) <- this just works, i like it
* [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
