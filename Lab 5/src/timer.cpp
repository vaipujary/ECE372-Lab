// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Assignment:  Lab 5
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//

#include "timer.h"

void initTimer1() {
  TCCR1A &= ~(1 << WGM10); 
  TCCR1A &= ~(1 << WGM11);
  TCCR1B |= (1 << WGM12); 
  TCCR1B &= ~(1 << WGM13);
}


void delayMs(unsigned int delay)
{
    unsigned int counter = 0;
    // turn on the clock by setting the prescaler bits to 64
    TCCR1B |= (1 << CS10) | (1 << CS11);
    TCCR1B &= ~(1 << CS12);

    while (counter < delay)
    {
        // Put the output compare match flag down  (OCF0A) by writing a logic 1
        TIFR1 |= (1 << OCF1A);
        TCNT1 = 0; // CLEAR the count register

        // poll the OCF0A flag until it is set or is logic 1
        while (!(TIFR1 & (1 << OCF1A)))
            ; // do nothing while the flag is down
        // after the flag has been raised 1 ms time has elapsed
        counter++;
    }

    // after exiting the larger while loop the time elasped has been 1ms*delay
    // turn off the Timer0 clock
    TCCR0B &= ~((1 << CS10) | (1 << CS11));
    TCCR0B &= ~(1 << CS12);
}