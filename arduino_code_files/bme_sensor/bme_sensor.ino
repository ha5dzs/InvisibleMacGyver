/*
 * BME sensor example.
 * We will read the sensor data, and send it back via the UART.
 */

#include <Wire.h> // Include Arduino's I2C library
#include <Adafruit_BME280.h> // Include the BME sensor library.

// defines for the pin names
#define SCL_PIN D5
#define SDA_PIN D6

Adafruit_BME280 bme_sensor; // This is the sensor's C++ object.

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200); // Initialise the UART
  Serial.println(); // Do an empty new line

  Wire.begin(SDA_PIN, SCL_PIN); // This initialises the I2C bus on the pins we assigned

  if(bme_sensor.begin())
  {
    // If we got here, the sensor got found.
    Serial.println("BME sensor found.");
  }
  else
  {
    // If we got here, the BME sensor didn't initialise.
    Serial.println("The BME sensor could not be initialised. Is it connected properly? Is it powered? Have you edited the header file?");
    while(1); // This makes sure nothing else happens
  }
  

}

void loop() {
  // put your main code here, to run repeatedly:
  /*
   * These are the print statements that provide data over the UART.
   * You should see these appearing in the serial monitor.
   */
  Serial.print("Temperature: ");
  Serial.print(bme_sensor.readTemperature());
  Serial.println(" °C");
  
  Serial.print("Humidity: ");
  Serial.print(bme_sensor.readHumidity()); // if you want to see Fahrenheit, you need to convert it manually
  Serial.println(" % RH");
  
  Serial.print("Air pressure is ");
  Serial.print(bme_sensor.readPressure()/100); // Sometimes conversion is this simple
  Serial.println(" hPa");

  Serial.println(); // Do an empty new line
  delay(2000); //This delay is here so it won't blast data at you

}
