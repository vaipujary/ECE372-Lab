// Authors:     Group 3   
// Date:        04/04/2024   
// Assignment:  Lab 4
//
// Description: 
//
// Requirements:
//      Use PORTC pins as outputs to drive the seven segment display
//      configured in a common cathode mode.
//----------------------------------------------------------------------//

#include <avr/io.h>
#include "sevensegmentdisplay.h"
#include "timer.h"
#include "switch.h"

void initSevenSegment() {
    DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5) | (1 << DDC6) | (1 << DDC7);
}

void sevenSegmentDisplay(unsigned int a) {
    switch(a) {
    case 0:
    
    break;
    case 1:
    break;
    case 2:
    break;
    case 3:
    break;
    case 4:
    break;
    case 5:
    break;
    case 6:
    break;
    case 7:
    break;
    case 8:
    break;
    case 9:
    break;
    default:
    break;
    }
}



