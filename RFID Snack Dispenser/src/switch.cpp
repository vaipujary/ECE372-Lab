#include <avr/io.h>

/*
 * Initializes pull-up resistor on PD0 and sets it into input mode
 */
void initSwitchPD2()
{
    DDRD &= ~(1 << DDD2); // Set direction for input

    PORTD |= (1 << PORTD2); // Enable pull up resistor (stable input)

    EIFR |= (1 << INTF2); // External Interrupt Mask Register
    EIMSK |= (1 << INT2); // External Interrupt Flag Register
}