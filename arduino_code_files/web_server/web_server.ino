/*
 * This code shows how to connect to a WiFi access point.
 */

#include <ESP8266WiFi.h> // WiFi library
#include <ESP8266WebServer.h> // Web server library

// I guess this is not the most secure way of doing it :)
char* network_name = "Pipewellgate-2.4G";
char* password = "iamnotgivingyoumypassword";

ESP8266WebServer web_server(80); // Operates on Port 80

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

  web_server.on("/", serve_the_user); // Server path is root ("/"), and this is the function we execute
  web_server.begin();
  Serial.println("Web server started, now listening on Port 80.");
}

// This function is executed when there is a HTTP request.
void serve_the_user() {
  Serial.println("Serving incoming reauest now.");
  // This one can display any string, as long as it's proper HTML.
  web_server.send(200, "text/html", "<h1>It works!</h1>");
}
void loop() {
  // put your main code here, to run repeatedly:
  web_server.handleClient(); // This one makes sure that new HTTP requests are processed.

}
