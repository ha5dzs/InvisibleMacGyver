# Glossary

Hopefully this will alleviate the confusion, should you have any...

## GPIO and TTL

GPIO stands for General Purpose Input/Output.  
The idea is that the function of a pin can be programmatically changed, to be:
- Digital output: it be set to logic levels 0 and 1
- Digital input: the pin can accept different logic levels. If you don't connect anything to a TTL-pin, it is considered to be on a logic 1, because of its internal pull-up resistor.
- High impedance/analogue input: the pin is detached from the microcontroller internally, or an analogue voltage may be measured using an ADC.

The electrical standard of the GPIO pins are TTL (Transistor-Transistor Logic, dating back from the '70s or so), **the NodeMCU board uses 3.3V, low-voltage TTL signals**, and there is no overloading protection!  
[Look at this](https://www.analog.com/media/en/training-seminars/tutorials/MT-098.pdf) if you want to read more about TTL, but it's not for the faint of heart.

In general:
- One TTL output can drive up to around 10 TTL inputs  
This is called **fan-out**, and the actual number depends on the inputs themselves and the maximum frequency you want to drive them at
- A Hi-Z pin has the impedance in range of kiloohms
- The physical wiring must have five times the bandwidth of the maximum frequency signal you plan to use. So don't expect a 40 MHz signal to propagate on some bushy hookup wire set-up on a breadboard! :)

## ADC/DAC

ADC stands for Analogue-to-digital converter. DAC stands for digital-to-analogue converter. The purpose of these devices are to interface between the real world where a signal can have any value, and the digital world where a value can have of discrete values in time and magnitude. Creating decent ADCs and DACs is a separate field in engineering, and is outside the scope of this project.  

There is one ADC available on the NodeMCU board. The specs are:
- Resolution: 10 bits (1024 levels)
- Lower reference: GND (0V)
- Higher reference: Vdd (3.3V) (some older boards only go up to 1V!)

There is no DAC installed on the NodeMCU board, but you can make several of them using PWM.

## PWM

PWM stands for Pulse Width Modulation.  
There is some information available on the [Arduino](https://www.arduino.cc/en/Tutorial/PWM) website about it as well, but very briefly:
- The PWM signal is a periodic TTL signal. Witin a single period, the signal has an on (1) state, and off (0) state.
- The term **duty cycle** determines that proportionally how much of the signal is logic 1 within the signal period.
Again, this is very superficial, and there is far more to it but it's outside the scope of this project.  
If the PWM frequency is high enough, and we change the duty cycle slowly enough, by filtering the PWM frequency, it is possible to get the slowly changing analogue signal.  
For example, you can set the PWM frequency at 50 kHz, you can change the duty cycle using an 1 kHz sine wave. If you attach a piezo speaker to the GPIO pin, you will hear the 1 KHz component. This is how [Class D amplifiers work](https://www.analog.com/en/analog-dialogue/articles/class-d-audio-amplifiers.html#).  

**In the NodeMCU, any GPIO pin can be a PWM output, using the `analogWrite()` function.** This is unlike other Arduino boards, where there are dedicated pins for PWM operation.

## I2C, IIC, or I squared C and I2S

I2C stands for Inter-Integrated Circuit Communication.  
It was invented by Philips in the 1980s, with one of the goals being to simplify their TVs and VCRs. Back then, integrating a full-fat computer system in consumer electronics was a challenge, because everything was in parallel: for example, a [8051-based system](https://en.wikibooks.org/wiki/Embedded_Systems/8051_Microcontroller) commonly found in on-screen display (OSD)-capable TVs of this era required external memory to store user settings. The data bus took up 10 pins (8 for data, plus chip select and read/write), and a bunch of other pins for addressing. Hooking up more peripherals, such as the infrared remote receiver or something that could decode [teletext](http://www.bighole.nl/pub/mirror/homepage.ntlworld.com/kryten_droid/teletext/spec/teletext_spec_1974.htm) meant that the physical writing of the main circuit board was large and complicated, i.e. more expensive.  

They invented this serial bus, where you could simply connect all your external peripherals to the same two (`SCL` or `SCK`: Clock, `SDA`: Data) wires, and you can communicate with your devices back and forth.  
However, Philips being Philips, they did a few weird things too: the clock speed is rather limited, and the addressing is only 7 bits. This means that there only ever can be 128 different devices. In reality, since it became a hit and took off, there are thousands of different I2C devices, so quite a few of them share the same address. Philips realised this and they came up with revised protocols with more available addresses, but there are nowhere near as many devices supporting them. So what happens when you have two devices on the same I2C bus? You may be able to do a hardware hack to change the address of one device, or, you can connect the offending device to a different bus.  

**In the Arduino and NodeMCU world, I2C is used to access low-speed devices, such as sensors and the IoT kit's included monochrome OLED screen. In the NodeMCU board included with the IoT kit, we use 3.3V power supply and logic levels, and the `SCL` and `SDA` pins could be assigned arbitrarily.** It's a healthy compromise between low pin count, low bandwidth, and limited performance.  

I2S is a similar to I2C, but it was invented in the 1990s, mainly to transfer digital sound, usually in CD players. This bus is now used in computer motherboards, to drive the on-board sound chip.

## SPI

SPI stands for Serial Peripheral Interface.
SPI was invented by Motorola in the 1980s, with the intention of reducing the pin count required in electronics with minimal compromise on performance. The idea is that the master controller would select the device to talk to, and would serially transfer the data it wishes to send. Simultaneously, while the master device sends data to the slave device it also can also upload its data, which can be processed by the master device.  

This kinda sounds complicated at first, but think of the data being sent and received as post-it notes attached to a hula hoop ring: The master and slave attaches their post-it notes, and then the master turns the hula hoop ring until the information gets exchanged.  

**The master selects the appropriate device with its `CS` pin, and the master also generates the clock pulses on the `SCLK` pin. The master sends data to the slave using the `MOSI` (Master Out Slave In) pin, and the master receives data from the slave using the `MISO` (Master In Slave Out) pin.** This means that the speed of the data transfer is set by the master device, and that a slave device can't directly initiate data transfer.  

**In the Arduino and NodeMCU universe, we use SPI for high-speed devices such as an SD card, a touch screen controller, or a high-resolution colour display. In the NodeMCU board, the the SPI can only be used with dedicated pins.**


## UART (or USART)

U(S)ART stands for Universal (Synchronous) Asynchronous Receiver Transmitter.

The UART was developed to be the electronic equivalent of a [teletype system](https://www.britannica.com/technology/teleprinter), which allowed digital information to be transferred on a serial bus. The term 'Asynchronous' means that it is capable of receiving and transmitting data at totally arbitrary times, without the need for any external syncronisation. The serial port lived its height at the [mainframe terminal](https://www.ibm.com/support/knowledgecenter/zosbasics/com.ibm.zos.znetwork/znetwork_261.htm) and the [dialup modem](https://www.itu.int/rec/T-REC-V.24-200002-I) era, and a lot of people wanted to kill it over the decades due to its slowness and clunkiness. However, the advantage of using an UART is its simplicity, and even the most basic computer systems (or if someone's desperate, even a human being, manually) can send data that a UART can receive.  
This made it a very useful tool with embedded systems: most routers have a built-in serial port console, and most bootloaders can be configured via the UART.  

**Using the UART to send diagnostic messages while developing a system is one of the best practices of [debugging](debug.md). In the NodeMCU board, we monitor the data sent by the UART over the USB link.**
