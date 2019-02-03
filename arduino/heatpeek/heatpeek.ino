#include <SPI.h>
#include <WiFi101.h>
#include <math.h>

// wifi SSID and password are defined in a separate file that isn't committed to git
#include "secrets.h" 
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// the server to report to
char server[] = "google.ca";

// the wifi client lets us make HTTP requests
WiFiClient client;

void setup() {
  // wait for serial port to connect. Needed for native USB port only
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  // for testing
  connectToWifi();
  String response = makeHttpRequest("/");
}

void loop() {
  // TODO: take temperature measurement
  // TODO: take action based on temperature

  // TODO: send the request to the server
  //connectToWifi();
  //makeHttpRequest();

  //Serial.println("Waiting between requests...");
  delay(2000);
}

/**
 * Attempts to connect to the wifi network named <ssid> using password <pass>
 * Returns true if successful, false otherwise
 */
bool connectToWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Already connected to " + String(ssid));
    return true;
  }

  Serial.println("Attempting to connect to " + String(ssid));
  long connectTime = millis();
  int status = WiFi.begin(ssid, pass);
  
  if (status == WL_CONNECTED) {
    Serial.println("Successfully connected to " + String(ssid) + " after " + String(millis() - connectTime) + "ms");
    printWifiConnectionInfo();
    return true;
  } else {
    if (status == WL_NO_SHIELD) {
      Serial.println("Failed to connect to " + String(ssid) + " after " + String(millis() - connectTime) + "ms. Could not find a wifi shield");
    } else {
      Serial.println("Failed to connect to " + String(ssid) + " after " + String(millis() - connectTime) + "ms");
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
 * Establishes a connection to <server> on port 80 and sends a GET request to <path>
 * Waits a maximum of 30s for a response from the server
 * Returns the entire HTTP response string, or empty string if something goes wrong
 */
String makeHttpRequest(String path) {
  Serial.println("\nAttempting to connect to " + String(server));
  long connectTime = millis();
  long requestTime;

  // send the request - connect(...) will block for up to 20s before timing out if server doesn't respond
  if (client.connect(server, 80)) {
    Serial.println("Successfully connected to " + String(server) + " after " + String(millis() - connectTime) + "ms");
    Serial.println("GET " + path + " HTTP/1.1");
    client.println("GET " + path + " HTTP/1.1");
    Serial.println("Host: " + String(server));
    client.println("Host: " + String(server));
    Serial.println("Connection: close");
    client.println("Connection: close");
    Serial.println();
    client.println();
    requestTime = millis();
  } else {
    Serial.println("Failed to connect to " + String(server) + " after " + String(millis() - connectTime) + "ms");
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
      Serial.println("Got a response from " + String(server) + " after " + String(millis() - requestTime) + "ms:");
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

