/*
 * This code creates a WiFi access point.
 */

#include <ESP8266WiFi.h> // WiFi library

//
char* network_name = "FostosMacska";
char* password = "NagyonFostosAMacska";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Starting the access point.");

  WiFi.softAP(network_name, password); // This one creates the access point

}

void loop() {
  // put your main code here, to run repeatedly:

}
