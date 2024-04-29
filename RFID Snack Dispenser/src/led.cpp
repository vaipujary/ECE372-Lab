// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Project:     RFID Snack Dispenser
//
// Description: This file handles the initialization of the two LEDs on the board. The green LED turns on when
// an authorized user accesses the snacks. The red LED turns on when an unauthorized user accesses the snacks.
// Both LEDs are off by default.

//----------------------------------------------------------------------//

#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

// initLED which returns void and takes no parameters
void initLED()
{
    // Set the data direction registers for port E
    DDRE |= (1 << DDE4) | (1 << DDE5);
}

// Turns on the Red LED
void turnOnRedLED()
{
    // Header pin 2
    PORTE |= (1 << PORTE4);
}

// Turns on the Green LED
void turnOnGreenLED()
{
    // Header pin 3
    PORTE |= (1 << PORTE5);
}

// Turns off the Red LED
void turnOffRedLED()
{
    // Header pin 2
    PORTE &= ~(1 << PORTE4);
}

// Turns off the Green LED
void turnOffGreenLED()
{
    // Header pin 3
    PORTE &= ~(1 << PORTE5);
}
