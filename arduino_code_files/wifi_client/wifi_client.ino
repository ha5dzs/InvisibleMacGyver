/*
 * This code shows how to connect to a WiFi access point.
 */

// Include the WiFi libraries
#include <ESP8266WiFi.h>

// I guess this is not the most secure way of doing it :)
char* network_name = "Pipewellgate-2.4G";
char* password = "iamnotgivingyoumypassword";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Start serial port
  Serial.println("");
  Serial.print("Network: ");
  Serial.println(network_name);

  // Initialise Wifi.
  WiFi.mode(WIFI_STA); // client mode
  // Add network details
  WiFi.begin(network_name, password); // Where to connect

  // Simple progress bar stolen from the the WiFiClient example
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected!");
  Serial.println("The device's IP address is: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
