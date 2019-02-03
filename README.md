# Heatpeek
An arduino-based thermostat that reports the local temperature to Prometheus via a wifi connection

## Compatibility
Heatpeek reports temperature changes to a [Prometheus](https://prometheus.io/) server via wifi. To connect to the wifi network, it makes use of the [WiFi101](https://www.arduino.cc/en/Reference/WiFi101) Arduino library. That library in turn supports the [Arduino MKR1000](https://www.arduino.cc/en/Guide/MKR1000) and the [Arduino WiFi Sheild 101](https://www.arduino.cc/en/Guide/ArduinoWiFiShield101).

To measure the temperature, the arduino sketch depends on the Adafruit MCP9808 library, designed for use with [Adafruit's MCP9808 breakout board](https://learn.adafruit.com/adafruit-mcp9808-precision-i2c-temperature-sensor-guide). When wiring the breakout board to your Arduino MKR1000, this [pinout diagram](https://components101.com/microcontrollers/arduino-mkr1000-wi-fi-board) may prove helpful.

Before the sketch can be compiled, you'll need to add the Adafruit MCP9808 library to your Arduino IDE. Search for it in the Sketch > Include Library > Manage Libraries... dialog box.

## Configuration
The arduino portion of Heatpeek is configured via constants that must be defined in `secrets.h`. For obvious reasons, this file is not committed to git. 

After cloning the repository, create the file in the `arduino/heatpeek` directory. Its contents should be as follows:
```
#define WIFI_SSID "The SSID of the wifi network to connect to goes here"
#define WIFI_PASSWORD "The WPA2 password or WEP key that is used to authenticate goes here"
#define PUSHGATEWAY_IP "The IP address of the Docker host that the push gateway container is running on goes here"
#define PUSHGATEWAY_PORT "The port that the Docker host is forwarding to the push gateway container goes here"
#define ROOM_NAME "A unique name for this sensor node goes here"
```

Once `secrets.h` is in place, the `heatpeek.ino` sketch can be opened in the Arduino IDE and uploaded to your board.

## Alternative Solutions
Some other projects that perform similar tasks and might be of interest to you include:
* [jckuester/weather-station](https://github.com/jckuester/weather-station)
* [larsp/co2monitor](https://github.com/larsp/co2monitor)
