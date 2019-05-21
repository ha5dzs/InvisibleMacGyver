# Supplementary material for MacGyvering in vision science: Interfacing systems that are not supposed to work together

The materials included here are the building blocks for the workshop of the same name on the [VSS 2019](http://visionsciences.org) conference.
**The target audience is people who know the basics of programming** (variables, loops, if statements, etc) **but haven't done any hands-on work with hardware before.**



# The freebie IoT (Internet of Things) kit

The kit consists of:
- A [NodeMCU V3](https://nodemcu.readthedocs.io/en/master/) board 
- A common cathode RGB led
- A [Bosch BME280](https://www.bosch-sensortec.com/bst/products/all_products/bme280) (temperature, humidity, and air pressure) sensor
- A [TCS23725](https://cdn-shop.adafruit.com/datasheets/TCS34725.pdf) RGBC sensor
- An [SSD1306](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)-based OLED screen
- An SD card breakout board with a 8 GB micro SD card
- Two [breadboards](https://learn.sparkfun.com/tutorials/how-to-use-a-breadboard/all) with suitable wires
...so if you didn't get your own, you can just buy the relevant bits. As of 2019, all these components are easily obtainable.  

The device is being programmed with [Arduino](http://www.arduino.cc) and its support libraries. I organised some examples and tutorials into various themes. All `.ino` files are loadable on their own.  

# [Ckick here for the workshop material](https://github.com/ha5dzs/InvisibleMacGyver/tree/master/WorkshopMaterial)

## Basics

- **[Getting started: Blinking the built-in LED](getting_started.md)**: If you are the tl;dr type, follow this and dive in! 
- [Programming in C](programming_in_c.md): This is a pretty superficial C-tutorial that only includes relevant bits to this material.
- [The NodeMCU V3 board](nodemcu_intro.md): What it is and why it is good.
- [Preparing your Arduino environment](arduino.md): Board support package and library management.

## Nitty-gritty

- [GPIO, ADC, I2C, SPI, PWM, and other fancy acronyms](glossary.md): and what they are used for.
- [Debugging your own code](debug.md): You will need this, no matter what you do.
- [Blinking an LED in weird colours](pwm.md): How to use the RGB LED.
- [Measuring the environment with the BME sensor](bme_code.md): Temperature, humidity, pressure.
- [Measuring colour temperature (not very accurately) with the TCS sensor](tcs_code.md): Using the Adafruit library.
- [Displaying graphics à la Adafruit GFX library](oled_code.md): Text, fonts, and animation.
- [Accessing the SD card](sd_code.md): Dumping data into files, kinda slowly.
- [Wifi on-board!](wifi-code.md): How to connect your thing to the Internet.
- [Concurrency: Interrupts and timers](concurrency.md): Not quite parallel computing, but close.

# Onwards and upwards

This hardware can be used for way more than just the examples shown above. Further examples are [included here](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/examples).
