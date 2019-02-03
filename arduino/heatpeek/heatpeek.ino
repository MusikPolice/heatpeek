#include <SPI.h>
#include <WiFi101.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_MCP9808.h>

// all of our configuration options are defined in a separate file that isn't committed to git
#include "secrets.h"

// the wifi client lets us make HTTP requests
WiFiClient client;

// the temperature sensor
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

void setup() {
  // make sure that the temperature sensor is connected
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check you connections and verify that it is configured to use I2C address 0x18!");
    while (1);
  }
  tempsensor.setResolution(3);

  // make sure that we can hit wifi
  if (!connectToWifi()) {
    Serial.println("Failed to connect to wifi!");
    while(1);
  }
}

void loop() {
  // take temperature measurement
  tempsensor.wake();
  float tempDegreesCelcius = tempsensor.readTempC();
  tempsensor.shutdown_wake(1);

  // send the temperature to prometheus
  connectToWifi();
  sendTemp(tempDegreesCelcius);
  
  delay(5000);
}

/**
 * Attempts to connect to the wifi network named <WIFI_SSID> using password <WIFI_PASSWORD>
 * Returns true if successful, false otherwise
 */
bool connectToWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Already connected to " + String(WIFI_SSID));
    return true;
  }

  Serial.println("Attempting to connect to " + String(WIFI_SSID));
  long connectTime = millis();
  int status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  if (status == WL_CONNECTED) {
    Serial.println("Successfully connected to " + String(WIFI_SSID) + " after " + String(millis() - connectTime) + "ms");
    printWifiConnectionInfo();
    return true;
  } else {
    if (status == WL_NO_SHIELD) {
      Serial.println("Failed to connect to " + String(WIFI_SSID) + " after " + String(millis() - connectTime) + "ms. Could not find a wifi shield");
    } else {
      Serial.println("Failed to connect to " + String(WIFI_SSID) + " after " + String(millis() - connectTime) + "ms");
      Serial.println("Are you sure that the correct SSID and password were provided?");
    }
    return false;
  }
}

/**
 * If connected to a wifi network, prints connection status information to the serial monitor
 */
void printWifiConnectionInfo() {
  if (WiFi.status() == WL_CONNECTED) {
    IPAddress ip = WiFi.localIP();
    Serial.print("My IP address is ");
    Serial.println(ip);
  
    long rssi = WiFi.RSSI();
    Serial.print("My signal strength (RSSI) is ");
    Serial.print(rssi);
    Serial.println(" dBm");
  } else {
    Serial.println("Not connected to a Wifi network");
  }
}

/**
 * Establishes a connection to <PUSHGATEWAY_IP> on port 80 and sends a GET request to <path>
 * Waits a maximum of 30s for a response from the server
 * Returns the entire HTTP response string, or empty string if something goes wrong
 */
String sendTemp(float temp) {
  long connectTime = millis();
  long requestTime;
  
  int port = atoi(PUSHGATEWAY_PORT);
  String host = String(PUSHGATEWAY_IP) + ":" + String(port);
  Serial.println("\nAttempting to connect to " + host);

  // send the request - connect(...) will block for up to 20s before timing out if server doesn't respond
  if (client.connect(PUSHGATEWAY_IP, port)) {
    Serial.println("Successfully connected to " + host + " after " + String(millis() - connectTime) + "ms");

    // see https://prometheus.io/docs/instrumenting/exposition_formats/
    String metricName = String(ROOM_NAME) + "_temperature";
    String requestBody = "#TYPE " + metricName + " gauge\n"
                       + metricName + " " + String(temp,4) + "\n";
    
    String request = "POST /metrics/job/heatpeek HTTP/1.1\n"
                   + String("Content-Type: text/plain; version=0.0.4\n")
                   + "Host: " + host + "\n"
                   + "Content-Length: " + String(requestBody.length()) + "\n"
                   + "\n"
                   + requestBody;
    
    Serial.println(request);
    client.println(request);
    requestTime = millis();
  } else {
    Serial.println("Failed to connect to " + host + " after " + String(millis() - connectTime) + "ms");
    return "";
  }

  // wait for up to 30s for the entire response to arrive
  String response = "";
  while (millis() - requestTime <= 30000) {
    while (client.available()) {
      byte b = client.read();
      if (b == -1) {
        break;
      }
      response = response + char(b);
    }

    if (response.length() > 0) {
      Serial.println("Got a response from " + String(PUSHGATEWAY_IP) + " after " + String(millis() - requestTime) + "ms:");
      Serial.println(response);
      break;
    } else {
      delay(100);
    }
  }

  // disconnect from the remote host
  if (!client.connected()) {
    client.stop();
  }
  
  return response;
}

