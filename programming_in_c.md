# Programming in C

Most low-level devices, like microcontrollers are programmed in the [C programming language](https://www.amazon.com/How-Program-International-Harvey-Deitel/dp/0273776843). Unlike PCs or smartphones, there is no operating system that takes care of [multitasking](https://www.linuxjournal.com/article/3910) or [hardware management](https://pocketnow.com/hardware-abstraction-layer). Some devices don't even have a display to write error messages to either, so you are pretty much are on your own when programming them.

Directly **programming the bare metal is** actually a **good** thing because:
- **You have all the available processing power** at your disposal:
Remember when the first Raspberry Pi came out? There was a famous [demo of the performance of the chip by playing Quake 3 on it](https://www.youtube.com/watch?v=lztrdudwHfM). They did this by stripping down the Linux OS to the bare minimum, so nothing else would drain resources.  

- **You decide what to utilise**:  
You don't always need every peripheral all the time. If you don't use them, you can simply not initialise them. This can reduce power drain and heat output.

- **You control the timing**:
Perhaps this is the most relevant to vision scientists. Everything happens in procedural programming, so it becomes easy to create precisely timed visual stimuli.

## Things that will probably look strange when you grew up using Matlab and Python

When Dennis Richie came up with the C programming language in 1972, computers were extremely limited in both available memory and performance. The structure and some features of the language reflects this.

### You have to compile your C code to Machine code

The process of compilation is the creation of machine-executable binary program code from the resource files you have included. This process actually happens when you use Matlab and Python too, but it's invisible to you. This means that there will be a slight discrepancy between your C-code and what actually happens inside the device, which then sparked the very famous [C versus assembly](http://www.dspguide.com/ch28/5.htm) debate.  

The consensus is that the same program written lower-level (Assembly or Iverson, or similar) performs better, at the price of being more complicated. In practice, now that computers are faster, it doesn't make much of a difference.

In Arduino, the C-compiler is included and is set up for you. You just have to press a button. If you are interested, here is some [documentation](https://gcc.gnu.org/onlinedocs/) on how things work, but it's not for the faint of heart!  

However, compiler error messages can be cryptic, which makes debugging a bit more difficult.

### C has more discipline

Terminate your lines with a semicolon. Curly brackets. Variables should not be declared mid-code. Just look at the examples and go with the flow. You will get into it eventually.

### C and C++

...are not the same thing, but they are very similar. The main advantage of C++ is that it allows object-oriented programming, just like in Python or Matlab. Any C++ compiler can handle C-code as well, and most C-compilers can handle C++ too. You don't have to worry about the language differences, especially when using the kit.

### C is case sensitive
Make sure that if you use a variable, or call a function, every name matches exactly. The variable `ReindeerFlotilla` is not the same as `reindeerFlotilla`.

### The `#include` directive

This is simiar to `import` in Python, Matlab doesn't really have an equivalent to this because you just include your relevant things in the `path`. If you want to use an external software library in C, you need its header `.h` file which contains the macros and variable type declarations, and you either need a `.c` file containing the code the header file refers to, or the `.dll`, `.so` or `.lib` or `.dylib` files that contain the relevant binary code.  

### `#define` `#ifdef`, `#ifndef` directives

Sometimes, you may want to have some flexibility in the code. For example, you may want to adjust something inside your code for development purposes, which you know you won't change during runtime. **Wasting memory is a sin** in devices like this, so it makes more sense to tell the compiler (well actually, the preprocessor) to copy and paste pieces of texts before generating the machine code. This is also useful when you want to change execution path, depending if a piece of hardware is present or not, for example.  

So, the `#define` directives are very similar to Excel macros.

### The dreaded address operators and pointers

This is kinda complicated, but once you wrap your head around it, it's very handy.

Back in the '70s when you didn't have enough resources in computers. The memory capacity was usually so small, you could easily manage it manually.  
A variable is found in the memory by its **address**, and the variable itself is stored as **data**. So in C, when you see something like:
```
unsigned int my_variable = 42; // Let's say this variable lives at 0x00FF
```
...you know tha the type of the variable is `unsigned int`, the data is 42. Let's say that in your compiled code, this variable is stored at the memory address number `255`, or as hexadecimal in a 16-bit address space, under the address `0x00FF`.  
The **address operator** gets the memory address of the variable, and the **pointer operator** has direct access to the data at the memory address it is pointing to:

```
unsigned int the_address_of_my_variable = &my_variable; // this value will be 255.
```
...and conversely, you can set up a pointer that points to the memory address of your variable, which is done this way:
```
unsigned int *my_pointer = 255; // This now points to the data at address 255.
```
Once you set up the pointer, you can access the data directly, with the pointer operator:
```
my_data_with_direct_pointer = *my_pointer; // and this value will be 42.
my_data_with_variable_address_pointer = *the_address_of_my_variable; // This is also going to be 42.
```

This is great, because you can have direct access to the memory, and you can manually access data in it. It's also quite fast, so you can use it to transfer a lot of data quickly.  
However, nothing stops you from setting the pointers the wrong way, and randomly wreak havoc inside the memory by writing garbage everywhere, or have access to stuff you were not supposed to.  
On modern computers, the OS safeguards the memory, and you are only allowed to write inside the piece of memory you are allocated to. If you attempt to read or write outside the allocated area, your code will cause an access violation (or segmentation fault) crash.  
On a microcontroller such as the ones Arduinos use there is no such safeguard system, so use these with caution.  

There was a very famous and similar bug in the OpenSSL library which is used in security applications. The software was allowed to read out data from the memory, including other people's passwords. There is an [xkcd comic](https://xkcd.com/1354/) about this, and you can do the same thing with pointers too: simply by moving the pointer inside the memory, you can read or write any data, anywhere.

### There can't be more than one return variable in a function

C only supports the return of one variable. This variable is intended to show if the operation undertaken by the function was successful or not. Usually, the return value of 0 means that the operation was successful, and everything else means that it had failed. You might have seen 'nonzero exit status' error messages before, and this is where it came from.  
If you want to manipulate some variables from within a function, you have some options:

- Access global variables from within the function.

This is normally bad practice on large systems, but it's OK for a microcontroller:
```
int global_variable = 0;

void main(void)
{
    int return_value = -1; // Local variable
    // This is the main branch of your code here
    return_value = my_function(); // This will adjust the global variable
}

int my_function(void)
{
    global_variable = 1;
    return 0; // the return value, indicating success.
}
```
- Pass on the memory addresses of the variables you want to manipulate to the function.
This is the proper way.  

In the main code, we pass the **address** of the variable, and in the function, we use the **data** at the address we passed on. The type of the pointer just tells us how many memory memory addresses the variable takes up. The exact number depends on the type of the variable, and the computer's architecture. The type used in the pointer and the type of variable you want to manipulate must match.
```
int global_variable = 0;

void main(void)
{
    int local_variable = -1;
    // Again, main branch of your code
    local_variable = my_function(&global_variable); // Pass on the address of the variable

}

int my_function(int *variable_to_manipulate)
{
    *variable_to_manipulate = 1; // Manipulate the data at the address we passed on
    return 0;
}
```
...and of course, if you don't want a function to return anything, then just declare it a `void` type.

### Arduino's handy C and C++ features

When using an Arduino-compatible hardware library to drive a peripheral (sensor, display, whatever), the associated functions are linked to the object you create. This way it will be possible to load the same library multiple times, and to organise code better.  
The Arduino development environment includes a lot of convenience features too: for example, changing digital output from 0 to 1 can be done with one simple function. There are plenty of examples included, but if you are curious, read the [Arduino documentation](https://www.arduino.cc/reference/en/).