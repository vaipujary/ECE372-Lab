// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Project:     RFID Snack Dispenser
//
// Description:
//
// Requirements:
//
//----------------------------------------------------------------------//

#include "timer.h"

void initTimer1(){
	// CTC setting in the timer register
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B |= (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);

    OCR1A = 2;                // 1ms timer at a prescalar of 8, CTC compare value
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayUs(unsigned int delay){
    unsigned int count = 0;

    // Turns on clock sets the prescaler bits to 8 (010)
    TCCR1B &= (1 << CS10) | (1 << CS12);
    TCCR1B |= (1 << CS11);

    while (count < delay) {         // 1 ms Delay
        TCNT1 = 0;                  // starting timer at 0
        TIFR1 |= (1 << OCF1A);      // set compare flag to start timer

        while (! (TIFR1 & (1 << OCF1A))) {};

        count++;
    }

    // Turns off clock
    TCCR1B &= ~((1 << CS10) | (1 << CS12));
    TCCR1B &= ~(1 << CS11);
}

void initTimer0()
{
    // intialize Timer0 for CTC mode of operation and interrupts
    // WGM00 = 0, WGM01 = 1, WGM02 = 0 for CTC mode
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    OCR0A = 249; // set comparator value for CTC mode
}

void delayMs(unsigned int delay)
{
    unsigned int counter = 0;
    // turn on the clock by setting the prescaler bits to 64
    TCCR0B |= (1 << CS00) | (1 << CS01);
    TCCR0B &= ~(1 << CS02);

    while (counter < delay)
    {
        // Put the output compare match flag down  (OCF0A) by writing a logic 1
        TIFR0 |= (1 << OCF0A);
        TCNT0 = 0; // CLEAR the count register

        // poll the OCF0A flag until it is set or is logic 1
        while (!(TIFR0 & (1 << OCF0A)))
            ; // do nothing while the flag is down
        // after the flag has been raised 1 ms time has elapsed
        counter++;
    }

    // after exiting the larger while loop the time elasped has been 1ms*delay
    // turn off the Timer0 clock
    TCCR0B &= ~((1 << CS00) | (1 << CS01));
    TCCR0B &= ~(1 << CS02);
}