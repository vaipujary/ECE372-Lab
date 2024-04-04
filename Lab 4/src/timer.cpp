// Authors:     Group 3  
// Date:        04/04/2024   
// Assignment:  Lab 4
//
// Description: 
//          NOTE: changes may still need to be made here
// Requirements:
//        1.Uses a timer0 to debounce states. Timer 1 to count 10 seconds for
//        the seven segment display.
//----------------------------------------------------------------------//

#include "timer.h"

void initTimer1(){
	// CTC setting in the timer register
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B |= (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);
}

void delayUs(unsigned int delay){
    unsigned int count = 0;

    OCR1AH = 0; 
    OCR1AL = 16; 

    // Turns on clock sets the prescaler bits to 8 (010)
    TCCR1B &= ~(1 << CS10) | (1 << CS12);
    TCCR1B |= (1 << CS11);

    while (count < delay) {         // 1 ms Delay
        TCNT1 = 0;                  // starting timer at 0
        TIFR1 |= (1 << OCF1A);      // set compare flag to start timer

        TCCR1B &= ~((1 << CS10) | (1 << CS12));
        TCCR1B |= (1 << CS11);

        while (! (TIFR1 & (1 << OCF1A))) {};

        count++;
    }

    // Turns off clock
    TCCR1B &= ~((1 << CS10) | (1 << CS12));
    TCCR1B &= ~(1 << CS11);
}

void initTimer0(){
    // CTC setting in the timer register
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);
}


void delayMs(unsigned int delay){
    unsigned int count = 0;

    // Turns on clock sets the prescaler bits to 64 (110)
    TCCR0B |= (1 << CS00) | (1 << CS01);
    TCCR0B &= ~(1 << CS02);

    while (count < delay) {         // 1 ms Delay
        TCNT0 = 0;                  // starting timer at 0
        TIFR0 |= (1 << OCF0A);      // set compare flag to start timer

        while (! (TIFR0 & (1 << OCF0A))) {};

        count++;
    }

    // Turns off clock
    TCCR0B &= ~((1 << CS00) | (1 << CS01));
    TCCR0B &= ~(1 << CS02);   
}