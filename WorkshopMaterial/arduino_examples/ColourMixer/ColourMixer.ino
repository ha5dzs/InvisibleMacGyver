/*
 * RGB colour mixer.
 * The three colour channels are D3, D2 and D1.
 * The common cathode is connected to ground.
 */

#include <math.h> // Mathematics library. Needed for square root.

// These are the defines for the colour channels.
#define RED D3
#define GREEN D2
#define BLUE D1

//#define DEBUG // Uncomment this line if you want to see debug messages from the functions.

#define MAX_PWM_VALUE 255 // By default on the NodeMCU board, it goes up to 1024, but let's be easy on the voltage regulator.

/*
 * The primary colours are stored as CIE 1931 chromaticity coordinates,
 * and the intensity is expressed as a fraction of the brightest component.
 */

// Put the primaries as variables here.
float red_x = 0.71;
float red_y = 0.29;
float red_intensity = 0.9;

float green_x = 0.13;
float green_y = 0.77;
float green_intensity = 0.8;

float blue_x = 0.12;
float blue_y = 0.06;
float blue_intensity = 1;

// This is the colour we are trying to display. This is roughly 2700K white.
float requested_colour_x = 0.45;
float requested_colour_y = 0.41;


// These are proportional values, and we will calculate the PWM values from these.
float red_drive = 0;
float green_drive = 0;
float bliue_drive = 0;

int triangleInterior(float test_point_x, float test_point_y, float point_A_x, float point_A_y, float point_B_x, float point_B_y, float point_C_x, float point_C_y)
{
  /*
   * This function tells whether a selected point is inside a triangle.
   * We use this to check if the requested colour is displayable,
   * (i.e. inside the colour gamut)
   * 
   * It returns 1 when the point is inside the triangle, and 0 when it's not.
   */
  // This is based on what's here:
  // http://mathworld.wolfram.com/TriangleInterior.html
  
  // Local variables.
  float denominator = 0;
  float value_a; // This is used for the final check
  float value_b; // This is also used for the final check
  int point_is_inside = -1; // If we don't touch this one, then the point is not inside the triangle

  //Assemble the denominator
  denominator = point_B_x * point_C_y - point_B_y * point_B_x;

  // Compute value_a
  value_a = ( (test_point_x * point_C_y - test_point_y * point_C_x) - (point_A_x * point_C_y - point_A_y * point_C_x) ) / denominator;
  // compute value_b
  value_b = ( (test_point_x * point_B_y - test_point_y * point_B_x) - (point_A_x * point_B_y - point_A_y * point_B_x) ) / denominator;

  // do the checks.

  if( (value_a > 0) && (value_b > 0) )
  {
    // If both values are positive
    if( (value_a + value_b) > 1 )
    {
      //..and their sum is greater than 1
      point_is_inside = 0; // then we are good to go.
      
    }
  }

  return point_is_inside;
}


float getMaxValue(float value_1, float value_2, float value_3)
{
  /*
   * This function returns the maximum value of the three specified.
   * We need this for the intensity calculations.
   */
  float interim_max_value = 0; // this is for the comparison.
  
  if(value_1 >= value_2)
  {
    // value_1 is greater or equal than value_2
    interim_max_value = value_1; // value_1 wins
  }
  else
  {
    interim_max_value = value_2; // value_2 wins
  }

  if(value_3 >= interim_max_value)
  {
    // value_3 is greater or equal to interim_max_value
    return value_3; // value_3 wins
  }
  else
  {
    return interim_max_value; // value_1 or value_2 wins, depending on the previous if statement
  }
   
}

void colourMixer(float * r_drive, float * g_drive, float * b_drive, // Output variables
              float colour_x, float colour_y, // Requested colour to calculate from
              float r_x, float r_y, float r_intensity, // Red primary
              float g_x, float g_y, float g_intensity, // Green primary
              float b_x, float b_y, float b_intensity) // Blue primary
{
  /* This function takes the requested colour, the primary colours and intensities, and updates the drive values.
   * Refer to the bundled matlab code (ColourMixer.m) to see more details on how this works.
   */

  // local variables
    // Red primary
  float red_distance = 0; // Distance between the primary and the chosen colour
  float red_line_length = 0; // Length of line between the primary and the opposide side of the triangle
  float red_line_slope = 0; // Slope of above line
  float red_line_intercept = 0; // Intercept point of above line
    // Green primary
  float green_distance = 0; // Distance between the primary and the chosen colour
  float green_line_length = 0; // Length of line between the primary and the opposide side of the triangle
  float green_line_slope = 0; // Slope of above line
  float green_line_intercept = 0; // Intercept point of above line
  int green_line_infinite_slope = 0; // If the slope of the green line is infinite, we need to know.
    // Blue primary
  float blue_distance = 0; // Distance between the primary and the chosen colour
  float blue_line_length = 0; // Length of line between the primary and the opposide side of the triangle
  float blue_line_slope = 0; // Slope of above line
  float blue_line_intercept = 0; // Intercept point of above line

    // Gamut sides. This is needed for the intersection point calculations.
  float blue_green_line_slope = 0;
  float blue_green_line_intercept = 0;
  int blue_green_line_infinite_slope = 0; // It may happen that the blue and green primaries share the same y coordinate.
  float red_blue_line_slope = 0;
  float red_blue_line_intercept = 0;
  float green_red_line_slope = 0;
  float green_red_line_intercept = 0;

    // Intersection points.
  float red_intersection_x = 0;
  float red_intersection_y = 0;
  float green_intersection_x = 0;
  float green_intersection_y = 0;
  float blue_intersection_x = 0;
  float blue_intersection_y = 0;

    // Interim drive values, before converting relative to 1.
  float red_interim_drive_value = 0;
  float green_interim_drive_value = 0;
  float blue_interim_drive_value = 0;
  float max_interim_drive_value = 0; // This one will be used by a separate getMaxValue function.


  // First of all, calculate the distances between the primaries and the chosen colour. Pythagoras' theorem.
  red_distance = sqrt( pow( (r_x - colour_x), 2 ) + pow( (r_y - colour_y), 2 ) );
  green_distance = sqrt( pow( (g_x - colour_x), 2 ) + pow( (g_y - colour_y), 2 ) );
  blue_distance = sqrt( pow( (b_x - colour_x), 2 ) + pow( (b_y - colour_y), 2 ) );

  // Calculate the line equations of the lines between the primaries and the chosen colour.
  red_line_slope = (colour_y - r_y) / (colour_x - r_x);
  red_line_intercept = colour_y - red_line_slope * colour_x;

  if(colour_x == g_x)
  {
    // If the x coordinates match, we have an infinite slope.
    green_line_infinite_slope = 1;
  }
  else
  {
    // If they don't match, we can continue normally.
    green_line_slope = (colour_y - g_y) / (colour_x - g_x);
    green_line_intercept = colour_y - green_line_slope * colour_x;
  
  }
  
  blue_line_slope = (colour_y - b_y) / (colour_x - b_x);
  blue_line_intercept = colour_y - blue_line_slope * colour_x;

  // Now we calculate the line equations of the edges of the colour gamut.

  if(b_x == g_x)
  {
    // If the x coordinates of the primaries match, we have an infinite slope.
    blue_green_line_infinite_slope = 1;
  }
  else
  {
    blue_green_line_slope = (b_y - g_y) / (b_x - g_x);
    blue_green_line_intercept = b_y - blue_green_line_slope * b_x;
  }
  red_blue_line_slope = (r_y - b_y) / (r_x - b_x);
  red_blue_line_intercept = r_y - red_blue_line_slope * r_x;
  green_red_line_slope = (g_y - r_y) / (g_x - r_x);
  green_red_line_intercept = g_y - green_red_line_slope * g_x;

  // Now that we got all the line equations, we can calculate the intersection points.
  
  if(blue_green_line_infinite_slope)
  {
    // If the blue and the green primaries are on the same x coordinate,
    red_intersection_x = g_x;
  }
  else
  {
    // If they don't share the x coordinate, continue normally.
    red_intersection_x = (blue_green_line_intercept - red_line_intercept) / (red_line_slope - blue_green_line_slope);
  }
  red_intersection_y = red_line_slope * red_intersection_x + red_line_intercept;
  if(green_line_infinite_slope)
  {
    // If the green line is vertical
    green_intersection_x = g_x;
  }
  else
  {
    // otherwise continue normally
    green_intersection_x = (red_blue_line_intercept - green_line_intercept) / (green_line_slope - red_blue_line_slope);
  }
  green_intersection_y = green_line_slope * green_intersection_x + green_line_intercept;
  blue_intersection_x = (green_red_line_intercept - blue_line_intercept) / (blue_line_slope - green_red_line_slope);
  blue_intersection_y = blue_line_slope * blue_intersection_x + blue_line_intercept;

  // Now that we know the intersection points, we can calculate the lengths of the lines. Adain, Pythagoras' theorem.
  red_line_length = sqrt( pow( (r_x - red_intersection_x), 2 ) + pow( (r_y - red_intersection_y), 2 ) );
  green_line_length = sqrt( pow( (g_x - green_intersection_x), 2 ) + pow( (g_y - green_intersection_y), 2 ) );
  blue_line_length = sqrt( pow( (b_x - blue_intersection_x), 2 ) + pow( (b_y - blue_intersection_y), 2 ) );

  /* Now we can calculate the drives of the LEDs. The drive is inversely proportional to the distance-length ratio.
   * We calculate the interim drive values first, as we need to use the largest one.
   * We also use the pointers to update the values in the code this function is being called from.
   * And this is where we take the different intensities into account.
   */

  red_interim_drive_value = (1 - (red_distance / red_line_length)) / r_intensity;
  green_interim_drive_value = (1 - (green_distance / green_line_length)) / g_intensity;
  blue_interim_drive_value = (1 - (blue_distance / blue_line_length)) / b_intensity;
  
#ifdef DEBUG
  Serial.print("red_interim_drive_value: ");
  Serial.println(red_interim_drive_value);
  Serial.print("green_interim_drive_value: ");
  Serial.println(green_interim_drive_value);
  Serial.print("blue_interim_drive_value: ");
  Serial.println(blue_interim_drive_value);
#endif
  
  // Find the maximum value out of these three
  max_interim_drive_value = getMaxValue(red_interim_drive_value, green_interim_drive_value, blue_interim_drive_value);

  // and now we are ready to push back the return values!
  *r_drive = red_interim_drive_value / max_interim_drive_value;
  *g_drive = green_interim_drive_value / max_interim_drive_value;
  *b_drive = blue_interim_drive_value / max_interim_drive_value;
  
  
  return;
}


void setup() {
  // put your setup code here, to run once:

  //local variables
  float red_drive = 0;
  float green_drive = 0;
  float blue_drive = 0;

  int red_pwm_value = 0;
  int green_pwm_value = 0;
  int blue_pwm_value = 0;


  Serial.begin(115200); // This is for debugging.
  Serial.println(""); 
  
  // Set pin directions 
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // Set PWM ang logic levels
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);

  Serial.print("Requested colour is: x = ");
  Serial.print(requested_colour_x);
  Serial.print(", y = ");
  Serial.print(requested_colour_y);
  Serial.println(".");

  // Check if the colour is displayable
  if(triangleInterior(requested_colour_x, requested_colour_y, red_x, red_y, green_x, green_y, blue_x, blue_y))
  {
    // If this is true, the point is not inside the triangle
    Serial.println("The requested colour is not displayable in the current gamut.");
  }
  else
  {
    // If we got here, we are good to go.
    Serial.println("The requested colour is inside the current gamut.");
    
  }

  // Call the colourMixer function, and get the normalised values out.
  colourMixer( &red_drive, &green_drive, &blue_drive, // We user pointer arguments the function can write into.
    requested_colour_x, requested_colour_y, // Requested colour coordinates
    red_x, red_y, red_intensity, // Red primary
    green_x, green_y, green_intensity, // Green primary
    blue_x, blue_y, blue_intensity // Blue primary
    );
#ifdef DEBUG    
  Serial.println("Red - Green - Blue drive values are:");
  Serial.print(red_drive);
  Serial.print(", ");
  Serial.print(green_drive);
  Serial.print(", ");
  Serial.print(blue_drive);
  Serial.println(".");
#endif

  red_pwm_value = round(red_drive * MAX_PWM_VALUE);
  green_pwm_value = round(green_drive * MAX_PWM_VALUE);
  blue_pwm_value = round(blue_drive * MAX_PWM_VALUE);
#ifdef DEBUG
  Serial.println("Red - Green - Blue PWM values are:");
  Serial.print(red_pwm_value);
  Serial.print(", ");
  Serial.print(green_pwm_value);
  Serial.print(", ");
  Serial.print(blue_pwm_value);
  Serial.println(".");
#endif

  // Put these PWM values, so we can see what we have done.
  analogWrite(RED, red_pwm_value);
  analogWrite(GREEN, green_pwm_value);
  analogWrite(BLUE, blue_pwm_value);

}

void loop() {
  // put your main code here, to run repeatedly:
/*
  analogWrite(RED, 255); // Set a random duty cycle between 0 and 100%
  analogWrite(GREEN, 162); // Set a random duty cycle between 0 and 100%
  analogWrite(BLUE, 55); // Set a random duty cycle between 0 and 100%
*/

}
