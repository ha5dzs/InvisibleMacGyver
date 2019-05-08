/*
 * This example uses the SD card in SPI mode.
 * We will write a file to the card,
 * and then read it back in.
 */

#include <SPI.h> // The SPI library
#include <SD.h> // SD card library.

// Chip Select pin.
#define CS_PIN D8

File the_file; // File object for creating and reading a file

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200); // We will use this for debugging.
  Serial.println(""); // Empty line

  // Initialise the SD card
  if(SD.begin(CS_PIN))
  {
    // If we got here, it's all good.
    Serial.println("SD card found.");
  }
  else
  {
    // If we got here, we couldn't initialise the card.
    Serial.println("Could not find the SD card. Is it plugged in? Is it wired up correctly? Is it powered?");
    while(1); // Make sure the code won't proceed.
  }

  // Check if the file exists, if so, delete
  if(SD.exists("the_file.txt"))
  {
    SD.remove("the_file.txt");
  }


  // Create a file
  the_file = SD.open("the_file.txt", FILE_WRITE);
  if(the_file)
  {
    Serial.println("File opened.");
  }
  else
  {
    // If we got here, we couldn't create the file.
    Serial.println("The file couldn't be created. Is your card read-only?");
    while(1);
  }

  // Write into the file
  the_file.println("This is some text in the file.");
  Serial.println("File written to.");

  // Close the file
  the_file.close();
  the_file.flush(); // Make sure the card finished saving stuff.
  Serial.println("File closed."); 


  // Now we read back the same file
  if(SD.exists("the_file.txt"))
  {
    // If it exists, open the file, read-only this time.
    the_file = SD.open("the_file.txt", FILE_READ);
    if(the_file)
    {
      // File is opened, all good.
      Serial.println("File opened for reading.");
    }
    else
    {
      // File couldn't be opened
      Serial.println("While the file exists, it can't be opened. Is something else using it?");
      while(1);
    }
  }
  else
  {
    // If we got here, the file we just created doesn't exist
    Serial.println("The file we just created doesn't seem to exist. Is your card working OK?");
    while(1);
  }

  // Read the file in:
  Serial.println("The file's content is:");
  while(the_file.available())
  {
    // This loop scans through the file and sends all the file data
    Serial.write(the_file.read()); // Blast the binary stream in the console!
  }
  Serial.println(""); // Empty new line

  the_file.close();
  Serial.println("File closed.");
}

void loop() {
  // put your main code here, to run repeatedly:

}
