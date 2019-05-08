/*
 * This code displays some of the features of the SSD1306 OLED screen.
 */

#include <Wire.h> // Include Arduino's I2C library
#include <Adafruit_SSD1306.h> // This is the Adafruit graphics library
#include <Adafruit_GFX.h> // Basic graphics library. Includes canvases, fonts, and others.

/*
 * Fonts. Fonts are cool.
 * https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
 */
#include <Fonts/FreeSans9pt7b.h> // This adds a font. You can change these.
#include <Fonts/FreeSans12pt7b.h> // Add an other font
#include <Fonts/FreeSerif24pt7b.h> // ...and yet an other one!
#include <Fonts/FreeMonoBold24pt7b.h> // more!

// defines for the pin names
#define SCL_PIN D5
#define SDA_PIN D6

//defines for the display. Many display uses the same controller.
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1

// Create the display's C++ object.
Adafruit_SSD1306 oled_display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

unsigned int number_to_increment = 0; // We will use to display this.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Initialise the UART
  Serial.println(); // Do an empty new line
  
  Wire.begin(SDA_PIN, SCL_PIN); // This initialises the I2C bus on the pins we assigned

  if(oled_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Check the docs for what this is
  {
    // If we got here, the display was found and is initialised.
    Serial.println("Display initialised.");
  }
  else
  {
    // If we got here, the library didn't find the OLED screen.
    Serial.println("The display could not be initialised. Is it connected properly? Is it powered?");
    while(1); // This makes sure nothing else happens
  }


  // Displays are fiddly.
  oled_display.clearDisplay();
  
  oled_display.setFont(); // Default font.
  oled_display.setTextColor(WHITE); // This is kinda stupid, but it makes sense with colour displays
  oled_display.setTextSize(1); // This is per-pixel scaling.

  oled_display.setCursor(0, 0); // Upper left corner.
  oled_display.print("Display initialised.");
  oled_display.display(); // If you ever used Psychtoolbox, this is Screen('Flip', w);

  Serial.println("Hardware set-up complete.");
}

void loop() {
  // put your main code here, to run repeatedly:

  // This is our first frame.
  delay(1000);
  oled_display.clearDisplay();
  oled_display.setCursor(0, 0);
  oled_display.setFont(); // default font.
  oled_display.setTextSize(1); // This is per-pixel scaling.
  oled_display.print("Long strings will wrap round the screen. It doesn't look very nice, but it can be used for debugging too.");
  oled_display.display();
  delay(2000);
  
  // Clear the screen
  oled_display.clearDisplay();
  oled_display.display();

  // This is our second frame.
  delay(1000);
  oled_display.clearDisplay();
  oled_display.setCursor(0, 0);
  oled_display.setTextSize(2);
  oled_display.print("2x scale");
  oled_display.setTextSize(1);
  oled_display.setCursor(0, 55);
  oled_display.setFont(&FreeSerif24pt7b);
  oled_display.print("Serif");
  oled_display.display();
  delay(2000);

  // Clear the screen
  oled_display.clearDisplay();
  oled_display.display();

  // This is our third frame. Inverted.
  delay(1000);
  oled_display.clearDisplay();
  oled_display.setTextSize(1);
  oled_display.setCursor(0, 40);
  oled_display.setFont(&FreeMonoBold24pt7b);
  oled_display.print("8-)");
    oled_display.invertDisplay(true);
  oled_display.display();
  delay(2000);

  // Clear the screen
  oled_display.clearDisplay();
  oled_display.invertDisplay(false);
  oled_display.display();

  // This is our fourth frame.
  delay(1000);
  oled_display.clearDisplay();
  oled_display.setFont();
  oled_display.setTextSize(1);
  oled_display.setCursor(0, 0);
  oled_display.println("First line");
  oled_display.println("Second line");
  oled_display.println("Third line");
  oled_display.setTextColor(BLACK, WHITE); // The next line will be inverted
  oled_display.println("Inverted text");
  oled_display.setTextColor(WHITE);
  oled_display.display();
  delay(3000);

  // Clear the screen
  oled_display.clearDisplay();
  oled_display.display();


}
