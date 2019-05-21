/*
 * This code pules a digital output
 * 100 microseconds, every 500 milliseconds or so.

 */
#include <Ticker.h>

#define PIN_TO_PULSE D2 // If you want to see what happens, put on an LED.

#define FLASH_LENGTH 200 // Microseconds (approximately)

Ticker function_executor;

void setup() {
  // put your setup code here, to run once:

  pinMode(PIN_TO_PULSE, OUTPUT);

  // This is to check if it works.
  digitalWrite(LED_BUILTIN, 0); // Turn on the LED
  delay(500); // delay 200 milliseconds
  digitalWrite(LED_BUILTIN, 1); // Turn off the LED


  digitalWrite(LED_BUILTIN, 0); // Turn on the LED
  digitalWrite(LED_BUILTIN, 1);
  // This takes 800 nanoseconds
  
  function_executor.attach_ms(500, pulsePin);

}


void pulsePin(void) {
  /*
   * This function is triggered by the Ticker, and does the flashing.
   * When measured with the oscillopscope, this delay is about 5.8 microseconds longer.
   */

  digitalWrite(PIN_TO_PULSE, 1); //Turn on the LED
  delayMicroseconds(FLASH_LENGTH);
  digitalWrite(PIN_TO_PULSE, 0); // Turn off the LED
 
  
}


void loop() {
  // put your main code here, to run repeatedly:

}
