// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Assignment:  Lab 5
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//

#include <avr/io.h>
#include <avr/interrupt.h>
/*
 * Initializes pull-up resistor on PD2 and sets it into input mode
 */
void initSwitchPD2()
{
    DDRD &= ~(1 << DDD2); // Set direction for input

    PORTD |= (1 << PORTD2); // Enable pull up resistor (stable input)

    EIFR |= (1 << INTF2); // External Interrupt Mask Register
    EIMSK |= (1 << INT2); // External Interrupt Flag Register
}