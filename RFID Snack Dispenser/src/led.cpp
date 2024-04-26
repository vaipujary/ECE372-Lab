#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

// initLED which returns void and takes no parameters
void initLED()
{
    // Set the data direction registers for port E
    DDRE |= (1 << DDE4) | (1 << DDE5);
}

// turns on the Red LED
void turnOnRedLED()
{
    // header pin 2
    PORTE |= (1 << PORTE4);
}

// turns on the Green LED
void turnOnGreenLED()
{
    // header pin 3
    PORTE |= (1 << PORTE5);
}

// turns off the Red LED
void turnOffRedLED()
{
    // header pin 2
    PORTE &= ~(1 << PORTE4);
}

// turns off the Green LED
void turnOffGreenLED()
{
    // header pin 3
    PORTE &= ~(1 << PORTE5);
}
