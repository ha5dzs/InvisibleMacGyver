# Concurrency: interrupts and timers.

Single-core computers can only ever execute one operation at a time. The illusion of multi-tasking (aka 'concurrency') with these kinds of computers is achieved via time division. In a decent computer, where there is an operating system, the kernel takes care of resource allocation, or scheduling. Scheduling resources is a branch of science itself, and some schedulers [with really weird names](http://ck.kolivas.org/patches/bfs/bfs-faq.txt) are being used.

In a microcontroller, such as the one the NodeMCU uses, concurrency without a scheduler can be easily achieved with interrupts.  
In Arduino, during normal operation, the `loop()` function runs. When -previously properly set up- interrupt is triggered, the processor saves (also called a `push` operation) all the variables to piece of memory called the stack, and executes the interrupt function with a clean environment. Once the interrupt function has finished, the variables are being recalled (also called a `pop` operation) back from the stack and everything continues like if nothing happened.

# Timers

Since every computer system has some sort of a clock, measuring time is quite easy: all you have to do is to measure the number of clock cycles or CPU cycles ([these are not always the same](https://www.difference.wiki/machine-cycle-vs-clock-cycle/)), and so something else at every nth cycle.  

In the NodeMCU, we have several libraries, but we will use the **Ticker library**.  

After [installing this library](arduino.md), here is how to use it:

```
#include <Ticker.h> // This is the header file for the Ticker library

Ticker my_timer; // This creates the Ticker object that we will use.

void setup()
{
    /*
     * Initialise your hardware here as usual
     */
    
    // Execute 'timed_function' every 30 seconds.
    my_timer.attach(30, timed_function);

}

void loop()
{
    /*
     * Do something you would normally do here
     */
}

void timed_function()
{
    /*
     * This function is being executed every time the Ticker object says so.
     */
}
```

The `my_timer.attach(...)` statement attaches the timer. Every time the specified time is up, it executes `timed_function`. You can use as many of these Ticker objects as you like. it will occupy memory. The Arduino system will let you know if you run out.

# GPIO pin-based interrupts

Interrupts can be requested externally too. This is useful when you want to control code execution based on some real-world event, such as a keypad press. The principle is the same: The triggered interrupt will execute the dedicated service function, and it will continue with normal operation once it is finished.  
These external interrupts can be triggered by:

- `RISING` edge, when the signal on the GPIO pin goes from logic 0 to logic 1
- `FALLING` edge, when the signal on the GPIO pin goes from logic 1 to logic 0
- `CHANGE`, which means either edge can trigger the interrupt.

In Arduino, the interrupt assignment is done with `AttachInterrupt()` and `DetachInterrupt()`. During runtime, all interrupts can be temporarily disabled (aka 'masked') with the `noInterrupts()` and the `interrupts()` functions respectively.

**You can use ANY digital pins with the NodeMCU board to set an interrupt**. This is not the case on other Arduino models, and some of them require [have additional assignments](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/) too.

**When your interrupt function is executing, the microcontroller will literally not to anything else.** It won't send data through the UART, or won't send a packet over the network.

Here is how to do external interrupts.
```
void setup()
{
    pinMode(D2, INPUT_PULLUP); // Make it an input, and pull it internall to logic 1.

    // This one assings the GPIO pin, the function and the edge together
    attachInterrupt(digitalPinToInterrupt(D2), interrupt_function, FALLING);
}

void loop()
{
    /*
     * Put your code here that you want to execute normally.
     * You can also leave this empty.
     */
}

void interrupt_function()
{
    /*
     * This function gets executed in the VERY MOMENT the D2 pin
     * is connected to the ground.
     * Depending on your application, you may need to
     * disable interrupts while you are serving an interrupt.
     */

    noInterrupts(); // mask all the interrupts

    /*
     * Add your interrupt service code here
     */

     interrupts(); // un-mask all the interrupts

}
```