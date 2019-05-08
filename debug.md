# Debugging your own code

Before the invention of the Arduino, getting started with a microcontroller was difficult, often taking up hours of work. This **was** because:  

- **The device needed to be powered properly**:
Very often, a separate power supply circuitry is needed, along with the appropriate filtering. Otherwise, connecting something to it or even just touching the board could cause a reset.  
- **The clock source had to be configured properly**:
Every processor needs a clock. Is it an internal RC oscillator? A separately installed crystal? Or perhaps the chip is receiving an external TTL pulse for clock? Does this clock frequency need dividing or multiplying?
- A number of **hardware** control registers **needed to be initialised properly**:
Are the [GPIO](glossary.md) pins inputs or outputs? How to scale the clock frequrency to get standard data rates to communicate with the rest of the world? Is there enough time for the ADC to acquire a sample before switching to an other channel? If the UART is used, won't anything else hijack its GPIO pins?

...and if you check [the documentation of a microcontroller](ww1.microchip.com/downloads/en/devicedoc/70138c.pdf), you will find that you can indeed do everything, once you familiarised yourself with the device after finding what you want from a document spanning hundreds of pages.

**In the Arduino, all the above steps are done for you, and you have the convenience of using a standardised system.** However, that doesn't mean you are out of the woods yet, it just means that you most probably won't need a multimeter or an oscilloscope to make sure that the circuit does what you want.

# This simplest option: [blinking an LED](getting_started.md)!
An LED is a very good tool to check code execution. You can turn it on and off in pretty much everywhere in your code. However, this comes at a cost of using a dedicated GPIO pin, which you can't use for anything else in the meantime.  
Furthermore, your perception is limited, so you may need to halt the code after reaching a particular point, like so:  

```
#define LED_BUILTIN D4 // The blue led on the NodeMCU board

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT); // Set the GPIO pin as output
    digitalWrite(LED_BUILTIN, 1); // Turn the LED off

    /*
     * Carry on with your hardware initialisation here
     */
}

void loop()
{
    /*
     * Do some important stuff here in your code
     */

     // ...and now, we turn the LED and halt execution!
     digitalWrite(LED_BUILTIN, 0); // This turns the LED on
     while(1); // This 'traps' the code in an infinite loop.

     /*
      * The stuff below this line will never get executed
      */
}
```

# Using the built-in [UART](glossary.md)

In the [NodeMCU board](nodemcu_intro.md) given with the IoT kit, there is only one hardware UART. This hardware UART is connected to the CH340 USB-to-Serial adapter, and is used for programming the device and by the ESP processor's bootloader. Once the code has been uploaded, the UART is released to the user, and can be used to pass on messages:

```
void setup()
{
    Serial.begin(115200); // The bit rate is 115200 bits per second
    
    /*
     * Do your hardware initialisation here
     */

     Serial.println("Hardware initialisation complete."); // This sends the message.
    

}

void loop()
{
    Serial.print("Woohoo, "); // This one is without a new line at the end
    Serial.println("this works!"); // This one does include the new line.

    delay(1000); // Sometimes you need to add delays so the UART won't keep blasting information at you

    /*
     * Carry on with your code here...
     */
}

```
You can view these messages with the Serial monitor, which is included with the Arduino. You can access it via the menu: `Tools -> Serial monitor`. Make sure you select the same bitrate as your code!  
It is worth mentioning that this may not be able to print out EVERY type of strings, so sometimes you have to assemble a line from several statements:
```

unsigned int loop_counter = 0; // This will incerase in every loop iteration

void setup()
{
    Serial.begin(115200); // Init UART
}

void loop()
{
    Serial.print("The loop executed "); // First part
    Serial.print(loop_counter); // Second part
    Serial.println(" times."); // Third part
    
    delay(1000); // Add a delay, for good measure
    loop_counter++; // Increase loop counter by 1.
}
```

In serial monitor, this manifests something like this:
```
The loop executed 0 times.
The loop executed 1 times.
```
...and so on. The variable will overflow at 255, and will start from 0 again. Note how the spaces are added in the strings, how the variable is just specified directly, and that the last statement also adds the new line characters.

# The [SoftwareSerial](https://www.arduino.cc/en/Reference/SoftwareSerial) library

Sometimes, you have to use the UART for some hardware you are controlling. This could be a modem or a GPS receiver, or something completely different. In these cases, you can make use of the SoftwareSerial library, which is bundled with the Arduino. As with anything else using [bit banging](https://www.dnatechindia.com/Tutorial/8051-Tutorial/BIT-BANGING.html), use it with caution.

Example code:
```
#include <SoftwareSerial.h> // Header file for the library

/*
 * This one creates the software serial port object.
 * You can pretty much use any pin for this.
 * Once the object is created, you can use it just like the real thing!
 */

SoftwareSerial the_software_uart(D0, D1); // D0: RX, D1: TX

void setup()
{
    the_software_uart.begin(115200); // Initialise, with this bitrate

    /*
     * Carry on with your initialisation here
     */
}

void loop()
{
    /*
     * You can just use the softwareserial object normally.
     */

    the_software_uart.print("Woohoo, ");
    the_software_uart.println("this works!");

    delay(1000); // Add a delay
}
```

# More advanced tools

With the above two tools, you can pretty much do everything. Sometimes you may need additional instruments like an oscilloscope or a logic analyser to investigate operation, or higher-level debug tools. These are outside the scope of the workshop. But you literally can [spend](https://www.tek.com/oscilloscope/tds2000-digital-storage-oscilloscope) a [fortune](https://www.keysight.com/en/pc-1000001966%3Aepsg%3Apgr/logic-analyzer?cc=US&lc=eng) on [these](https://visualgdb.com/tutorials/esp8266/nodemcu/jtag/) things!