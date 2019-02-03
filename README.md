# heatpeek
An arduino-based thermostat that reports the local temperature to Prometheus via a wifi connection

## Compatibility
Heatpeek reports temperature changes to a [Prometheus](https://prometheus.io/) server via wifi. To connect to the wifi network, it makes use of the [WiFi101](https://www.arduino.cc/en/Reference/WiFi101) Arduino library. That library in turn supports the [Arduino MKR1000](https://www.arduino.cc/en/Guide/MKR1000) and the [Arduino WiFi Sheild 101](https://www.arduino.cc/en/Guide/ArduinoWiFiShield101).

## Configuration
The arduino portion of Heatpeek is configured via constants that must be defined in `secrets.h`. For obvious reasons, this file is not committed to git. 

After cloning the repository, create the file in the `arduino/heatpeek` directory. Its contents should be as follows:
```
#define SECRET_SSID "The SSID of the wifi network to connect to goes here"
#define SECRET_PASS "The WPA2 password or WEP key that is used to authenticate goes here"
```

Once `secrets.h` is in place, the `heatpeek.ino` sketch can be opened in the Arduino IDE and uploaded to your board.
