/*
 * RGB Blink.
 * The three colour channels are D0, D1 and D3.
 * The common cathode is connected to D2.
 */


// These are the defines for the colour channels.
#define RED D3
#define GREEN D1
#define BLUE D0
#define C_CATHODE D2

void setup() {
  // put your setup code here, to run once:
  
  // Set pin directions 
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(C_CATHODE, OUTPUT);

  // Set PWM ang logic levels
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  digitalWrite(C_CATHODE, 0);

  
  randomSeed(8472);  // Change the seed of the random-number generator

}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(C_CATHODE, 0); // turn on the LED
  analogWrite(RED, random(0, 255)); // Set a random duty cycle between 0 and 100%
  analogWrite(GREEN, random(0, 255)); // Set a random duty cycle between 0 and 100%
  analogWrite(BLUE, random(0, 255)); // Set a random duty cycle between 0 and 100%

  delay(500);

  digitalWrite(C_CATHODE, 1); // Turn off the LED
  
  delay(500);

}
