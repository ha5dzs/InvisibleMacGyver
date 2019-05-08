#define THE_ONBOARD_BLUE_LED D4

void setup() {
  // put your setup code here, to run once:
  pinMode(THE_ONBOARD_BLUE_LED, OUTPUT); // GPIO 2
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(THE_ONBOARD_BLUE_LED, HIGH); // Turn the LED off
  delay(500);
  digitalWrite(THE_ONBOARD_BLUE_LED, LOW); // Turn the LED on
  delay(500);
}
