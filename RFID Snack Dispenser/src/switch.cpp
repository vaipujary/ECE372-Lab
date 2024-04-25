#include <avr/io.h>

/*
 * Initializes pull-up resistor on PD0 and sets it into input mode
 */
void initSwitchPD0()
{

    // Initializes pin 50 direction on board as an input (button)
    PORTD |= (1 << PD0);  // PD0 must be initialized with an input pull-up resistor
    PORTD |= (1 << PORTD0); // Enable pullup resistor by writing 1 to the port.

    // Pin-Change Interrupts must be enabled for pin PD0
    PCICR |= (1 << PCIE0);      // Enables PCINT 7:0
    PCMSK0 |= (1 << PCINT0);    // Enables PCINT for PCINT3 (pin PD0)
}