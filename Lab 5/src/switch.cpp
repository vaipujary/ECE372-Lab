// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Assignment:  Lab 5
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//

#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB3()
{
    DDRB &= ~(1 << DDB3); // set direction for input

    PORTB |= (1 << PORTB3); // enable pull up resistor (stable input)

    // set up for pin change interrupt
    //  enable PCINT 0 - 7
    PCICR |= (1 << PCIE0);
    // enable PCINT3 since we're using switch 3
    PCMSK0 |= (1 << PCINT3);
}