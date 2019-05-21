/*
 * This is origninally implemented with the NodeMCU board.
 * It uses a:
 * -SSD1306 OLED display, and
 * -TCS34725 RGB sensor module.
 * ...and their corresponding Adafruit libraries.
 */

#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/*
 * Before we begin, a few things to mention:
 * If you are using the OLED screen, make sure you change your library settings.
 * -In Adafruit_SSD1306.h somewhere around line 73, if you are using the 128x64 module, uncomment:
 * #define SSD1306_128_64
 */

Adafruit_SSD1306 display(15); //Display reset is pin 5, but it's not used in the ESP module.
Adafruit_TCS34725 tcs_sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X); //The tcs sensor.

uint16_t r_adc_value, g_adc_value, b_adc_value, c_adc_value; //These are 16-bit variables
int gain_level = -1;
float xc, yc, T; //CIE 1931 coordinates, and T for the colour temperature, where applicable.

void setup() {
  //We need i2c to talk to the peripherals.
  Wire.begin(2, 0); //GPIO2 is SDA, and GPIO0 is SCL.

  tcs_sensor.begin(); //Initialise the sensor.

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Create the 5 Volts internally, my display is at address 0x3C
  display.clearDisplay(); //Clears the frame buffer from power-up related garbage
  display.display(); //Put the framebuffer's contents to the display
  display.setTextColor(WHITE); //The OLED is monochrome, but the Adafruit GFX library needs this apparently.

  //Debug messages on serial port.
  Serial.begin(115200);
  
}

void loop() {
  gain_level = optimise_gain(gain_level); //update the gain level.
  tcs_sensor.getRawData(&r_adc_value, &g_adc_value, &b_adc_value, &c_adc_value); //Get data from the sensor.
  calculate_chroma_coords(&xc, &yc, r_adc_value, g_adc_value, b_adc_value); //Calculate the CIE 1931 coordinates.njgggggggggggggtr
 
  // put info on the display.
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Sensor gain level: ");
  display.println(gain_level); //This returns the optimal gain level.

  display.print("R: ");
  display.print(r_adc_value);
  display.print(" G: ");
  display.println(g_adc_value);
  display.print("B: ");
  display.print(b_adc_value);
  display.print(" C: ");
  display.println(c_adc_value);

  display.print("x: ");
  display.println(xc);
  display.print("y: ");
  display.println(yc);
  
  display.display();
}

int optimise_gain(int gain) {
  /*
   * This function reads the ADC values from the pre-initialised TCS34725 sensor,
   * and based on the values of the clear channel, it adjusts the sensor gain.
   * Inpout and output argument is gain level, which is an int ranging from 0 to 3.
   */
  uint16_t red_adc_value, green_adc_value, blue_adc_value, clear_adc_value; //ADC values go here. 16-bit number, between 0 and 65535.
  /*
   * These two numbers set the gain control hysteresis. They were chosen arbitrarily.
   * If the ADC is too high, the sensor might clip.
   * If the ADC value is too low, the values will be subject to quantisation noise.
   */
  uint16_t adc_low_threshold = 8192;
  uint16_t adc_high_threshold = 61439;
   
  unsigned gain_is_changing = 1; //Gain control exit semapore.
  
  while(gain_is_changing) {
    //read from the sensor. I have to do it this way, for some reason, null pointers will reset my ESP module.
    tcs_sensor.getRawData(&red_adc_value, &green_adc_value, &blue_adc_value, &clear_adc_value); //We only care about the clear channel.
    Serial.println("optimise_gain(): ADC values");
    Serial.print("optimise_gain(): R: ");
    Serial.println(red_adc_value);
    Serial.print("optimise_gain(): G: ");
    Serial.println(green_adc_value);
    Serial.print("optimise_gain(): B: ");
    Serial.println(blue_adc_value);
    Serial.print("optimise_gain(): C: ");
    Serial.println(clear_adc_value);
    //Change the ADC gain 
    if(clear_adc_value < adc_low_threshold) {
      //Signal too weak, crank up gain.
      gain_is_changing = 1;
      gain++; 
    }
    else {
      //If we got here, we didn't need to adjust the gain.
      gain_is_changing = 0;
    }
    if(clear_adc_value > adc_high_threshold) {
      //signal too strong, reduce gain
      gain_is_changing = 1;
      gain--;
    }
    else {
      //if we got here, we didn't need to adjust the gain.
      gain_is_changing = 0;
    }
  
    //Make sure we stay in range
    if(gain < 0) {
      //If we got here, the sensor gets some brutal amount of light. Can't be helped from software.
      gain = 0;
    }
    if(gain > 3) {
      //If we got here, the sensor is in a very dark environment. Can't be helped from software.
      gain = 3;
    }
    Serial.print("optimise_gain(): Gain is set to level ");
    Serial.println(gain);
    
    //We now need to re-configure the sensor accordingly. This could be neater, but I wanted to return as an int.
    switch(gain) {
      case 0:
        tcs_sensor.setGain(TCS34725_GAIN_1X);
        break;
      case 1:
        tcs_sensor.setGain(TCS34725_GAIN_4X);
        break;
      case 2:
        tcs_sensor.setGain(TCS34725_GAIN_16X);
        break;
      case 3:
        tcs_sensor.setGain(TCS34725_GAIN_60X);
        break;
      default:
        //this case should never happen.
        break;
    }
  }

  return gain; //This returns the gain level.
  
}

void calculate_chroma_coords( float *out_x, float *out_y, uint16_t red_adc_value, uint16_t green_adc_value, uint16_t blue_adc_value) {
  /*
   * This function reads from the pre-initialsied tcs sensor, and calculates the CIE 1931 coordinates.
   */
  //the primary colour chromaticity coordinates can be determined from the datasheet.
  float primary_r_x = 0.582692;
  float primary_r_y = 0.382919;
  float primary_g_x = 0.283233;
  float primary_g_y = 0.525670;
  float primary_b_x = 0.144560;
  float primary_b_y = 0.134175;

  //We need to compensate for the different sensitivities in each colour channel.
  //The gain control function makes sure we won't clip our ADC values.
  float r_luminance, g_luminance, b_luminance, sum_luminance; //These are the Y values for each colour channel, and their sum.

  //Step 1. Get the ADC values.
  r_luminance = float(red_adc_value);
  g_luminance = float(green_adc_value);
  b_luminance = float(blue_adc_value);

  //The output chromaticity coordinates are calculated from the ratio of the three components.

  //Step 2. Calculate the mean chromaticity coordinates. This is computationally intensive, but it can't be helped.
  sum_luminance = r_luminance + g_luminance + b_luminance;
  *out_x = ( r_luminance / sum_luminance ) * primary_r_x + ( g_luminance / sum_luminance ) * primary_g_x + ( b_luminance / sum_luminance ) * primary_b_x;
  *out_y = ( r_luminance / sum_luminance ) * primary_r_y + ( g_luminance / sum_luminance ) * primary_g_y + ( b_luminance / sum_luminance ) * primary_b_y;

  //done!
  
}
