// Authors:     Group 3 Alicia Enriquez   
// Date:        04/04/2024   
// Assignment:  Lab 4
//
// Description: 
//
// Requirements:
//        1.Uses a switch to turn the motor off.
//        2.The external interrupt must be of the type INTn (not PCINT) that is
//        used for switch debouncing. The interrupt sense control should be
//        configured for any logical change on INTn generates an interrupt
//        request.
//        3.PORTD0 should be used for this external interrupt.
//        4.The external interrupt should not work while the timer and seven
//        segment display is counting down from 9 secs to 0.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

void initSwitchPD0(){
    // Initializes pin 21 direction on board as an input (button)
    PORTD |= (1 << PD0);  // PD0 must be initialized with an input pull-up resistor
    DDRD &= ~(1 << DDD0);  // Sets pin direction as an input (pin D0)

    // External Interrupts must be enabled for pin PD0
    EIFR |= (1 << INTF0); // flag
    EIMSK |= (1 << INT0); // mask
}

void turnOffImsk() {
    EIMSK &= ~(1 << INT0);
}

void turnOnImsk() {
    EIMSK |= (1 << INT0);
}
