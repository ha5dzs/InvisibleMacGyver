# Let's make this board blink!

Even the most complicated and fancy projects start with a very simple step. Blinking an LED is the equivalent of `Hello, world!` in programming. In this case, we will blink the on-board blue LED on the NodeMCU board, so you will get an idea on how this thing behaves when the C-code compiled, the firmware was uploaded successfully, **AND the code doesn't crash during runtime** :).

---
### For tl;dr types:
- Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software), install [the ESP8266 board support package](arduino.md) to be used with the NodeMCU board, load the [included code](arduino_code_files/blink_test/blink_test.ino) into the editor, and verify whether it compiles. If you get stuck, [read this](arduino.md).  
- Connect the USB cable, verify if [you can see the serial port](nodemcu_intro.md).
- Select the right (NodeMCU 1.0 - ESP12E module) board and serial port in the 'tools' menu.
- Copy and paste the clode below or [download and open this `.ino` file](arduino_code_files/blink_test/blink_test.ino), select the 'NodeMCU' board, and the serial port.
- Put the board into flash mode the following way:
> 1., Press and hold the `RESET` button  
> 2., Press and hold the `FLASH` button  
> 3., Release the `RESET` button  
> 4., Release the `FLASH` button  
- Press the `Upload` (the one with the little arrow) button, wait for the to be compiled and uploaded to the board, and then witness the one-second-blinking action of the integrated blue LED.  

If you want to read more about this board, [read this](nodemcu_intro.md). For the adventurous types, there is a rather superficial and Arduino-focused [description of the C programming language](programming_in_c.md).

---

#### The anatomy of a typical Arduino code

For simple applications, the Arduino code has two distinct parts, which are organised into functions.  

The `setup()` function is used to initialise the hardware, and is being run first thing after the device has been powered up.  

Afterwards, the `loop()` function is being called repeatedly for the rest of the operation time.  

Just like in C, you can always add extra libraries and other code, simply by copying them into the directory you are working in.

## Blink

So, here is the code ([and you can download here](arduino_code_files/blink_test/blink_test.ino)), which makes the blue led blink about once a second:
```
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
```
The `#define` is a directive that substitutes text inside the C-code, and the `void` text before the functions indicate that they don't provide return values. These are specific to the C programming language [and you can read a bit more about it here](programming_in_c.md).  

When you have a piece of code like this, **you should always consider [the hardware the code is supposed to run on](nodemcu_intro.md)**. In this case, we know that **the LED is connected via a pull-up resistor, so it will only come on when the appropriate pin is pulled down to a logic zero**, towards ground.  

Load the code in your [environment that you have set up appropriately for this hardware](arduino.md). There are two buttons of interest in the toolbar:

![arduino buttons](images/arduino_buttons.png)  

The one on the left is the **compile** button, which creates the machine code, but it doesn't upload it to the device. You can use this button to check if your code is 'grammatically correct', so there are no immediate problems that makes the compiler complain.  

The button on the right is the **compile and upload** button, which compiles the code and uploads it to the device.

## Flash mode

Since [the NodeMCU device is being programmed the very same serial port you have access to as well](nodemcu.md), it was important to the designers to separate program an user modes. This is good, because it can keep things rather secure, so the hardware can't be reprogrammed without manual interaction. There [are other arduino boards](https://www.arduino.cc/en/main/boards) that do not have this feature.  

**In order to upload your code, you have to put the board into flash mode.** Here is how:  
While having the USB connected to your computer and [your drivers and environment being properly set up](arduino.md),  
- Press and hold the `RESET` button
- While pressing the holding the `RESET` button, press and hold the `FLASH` button too.
- Release the `RESET` button, while keep pressing the `FLASH` button
- Release the `FLASH` button

The goal of this operation is that the board detects the `FLASH` button being pressed during bootup. If you did this successfully, **the blue LED will blink once for a short time after the `RESET` button has been released**.  

Now press the upload button, and you should see something like this in the terminal window, with orange characters:
```
Uploading 289728 bytes from /var/folders/q7/2vs8j0w51pq6qjms00g3w97c0000gn/T/arduino_build_975950/blink_test.ino.bin to flash at 0x00000000
................................................................................ [ 28% ]
................................................................................ [ 56% ]
................................................................................ [ 84% ]
...........................................     
```

After this, the blue LED should blink. That's it!

### [Download code here](arduino_code_files/blink_test/blink_test.ino)

# Now you know the basics, head back to the [main page](README.md) and assemble some circuits. You can try them in any order, they don't depend on each other. The blinking code is now stored in the flash memory of the board, and will stay there for the next decade or so, or until you upload something else.