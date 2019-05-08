/*
 * This code initialises the TCS34725 RGB sensor,
 * and display's the environment colour temperature
 */

#include <Wire.h> // Include Arduino's I2C library
#include <Adafruit_TCS34725.h> // Include Adafruit's TCS library

// defines for the pin names
#define SCL_PIN D5
#define SDA_PIN D6

Adafruit_TCS34725 tcs_sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_16X);; // This is the sensor's C++ object.

// ADC values from the sensor
uint16_t red_value = 0;
uint16_t green_value = 0;
uint16_t blue_value = 0;
uint16_t clear_value = 0;

uint16_t colour_temperature = 0; // This is in Kelvins

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200); // Initialise the UART
  Serial.println(); // Do an empty new line

  Wire.begin(SDA_PIN, SCL_PIN); // This initialises the I2C bus on the pins we assigned

  if(tcs_sensor.begin())
  {
    // If we got here, the sensor got found.
    Serial.println("TCS sensor found.");
  }
  else
  {
    // If we got here, the TCS sensor didn't initialise.
    Serial.println("The TCS sensor could not be initialised. Is it connected properly? Is it powered?");
    while(1); // This makes sure nothing else happens
  }

  // Now we can continue with configuring the sensor
  tcs_sensor.setInterrupt(false); // This one disables the interrupt pin

  // Just for the hell of it, let's change the gain and the integration time.
  tcs_sensor.setGain(TCS34725_GAIN_4X); // Reduce this gain if you saturate your sensor
  tcs_sensor.setIntegrationTime(TCS34725_INTEGRATIONTIME_700MS);

  Serial.println("Hardware initialised.");
  
}

void loop() {
  // put your main code here, to run repeatedly:

  tcs_sensor.getRawData(&red_value, &green_value, &blue_value, &clear_value); // Update the value with pointers
  colour_temperature = tcs_sensor.calculateColorTemperature(red_value, green_value, blue_value); // Update the value as return value

  // Now we print
  Serial.println("R-G-B-C channel ADC values are: ");
  Serial.print(red_value); // Print ADC value
  Serial.print(", "); // Add a space
  Serial.print(green_value); // Print ADC value
  Serial.print(", "); // Add a space
  Serial.print(blue_value); // Print ADC value
  Serial.print(", "); // Add a space
  Serial.println(clear_value);
  
  Serial.print("Colour temperature is: ");
  Serial.print(colour_temperature);
  Serial.println(" Kelvin");

  Serial.println(""); // Print empty line
  delay(2000);

}
