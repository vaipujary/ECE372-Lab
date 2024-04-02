// Authors:     Group 3
// Date:        04/04/2024
// Assignment:  Lab 4
//
// Description:
//      https://www.electronicwings.com/arduino/7-segment-display-interfacing-with-arduino-uno
// Requirements:
//      Use PORTC pins as outputs to drive the seven segment display
//      configured in a common cathode mode.
//----------------------------------------------------------------------//

#include <avr/io.h>
#include "sevensegmentdisplay.h"
#include "timer.h"
#include "switch.h"

void initSevenSegment()
{
  DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5) | (1 << DDC6) | (1 << DDC7);
}

void sevenSegmentDisplay(int a) /* Function for displaying number (0-9) */
{
  switch (a)
  {
  case 0:
    PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) |
             (1 << PORTC3) | (1 << PORTC4) | (1 << PORTC5);
    PORTC &= ~(1 << PORTC6);
    break;
  case 1:
    PORTC |= (1 << PORTC1) | (1 << PORTC2);

    PORTC &= ~(1 << PORTC0) &
             ~(1 << PORTC3) & ~(1 << PORTC4) & ~(1 << PORTC5) & ~(1 << PORTC6);
    break;
  case 2:
    PORTC |= (1 << PORTC0) | (1 << PORTC1) |
             (1 << PORTC4) | (1 << PORTC6) |
             (1 << PORTC3);
    PORTC &= ~(1 << PORTC2) & ~(1 << PORTC5);
    break;
  case 3:
    PORTC |= (1 << PORTC0) | (1 << PORTC1) |
             (1 << PORTC2) | (1 << PORTC3) |
             (1 << PORTC6);
    PORTC &= ~(1 << PORTC4) & ~(1 << PORTC5);
    break;
  case 4:
    PORTC |= (1 << PORTC1) | (1 << PORTC2) |
             (1 << PORTC5) | (1 << PORTC6);
    PORTC &= ~(1 << PORTC0) & ~(1 << PORTC3) & ~(1 << PORTC4);
    break;
  case 5:
    PORTC |= (1 << PORTC0) | (1 << PORTC2) |
             (1 << PORTC3) | (1 << PORTC5) |
             (1 << PORTC6);
    PORTC &= ~(1 << PORTC1) & ~(1 << PORTC4);
    break;
  case 6:
    PORTC &= ~(1 << PORTC1);
    PORTC |= (1 << PORTC0) | (1 << PORTC2) | (1 << PORTC3);
    PORTC |= (1 << PORTC4) | (1 << PORTC5) | (1 << PORTC6);
    break;
  case 7:
    PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2);

    PORTC &= ~(1 << PORTC3) & ~(1 << PORTC4) & ~(1 << PORTC5) & ~(1 << PORTC6);
    break;
  case 8:
    PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) |
             (1 << PORTC3) | (1 << PORTC4) | (1 << PORTC5) | (1 << PORTC6);
    break;
  case 9:
    PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) |
             (1 << PORTC3) | (1 << PORTC5) | (1 << PORTC6);

    PORTC &= ~(1 << PORTC4);
    break;
  default:
    PORTC &= ~(1 << PORTC0) & ~(1 << PORTC1) & ~(1 << PORTC2) &
             ~(1 << PORTC3) & ~(1 << PORTC4) & ~(1 << PORTC5) & ~(1 << PORTC6);
    break;
  }
}
