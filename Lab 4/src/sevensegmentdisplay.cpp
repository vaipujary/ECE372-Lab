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

void initSevenSegment() {
    DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5) | (1 << DDC6) | (1 << DDC7);
}

void sevenSegmentDisplay(int a)	/* Function for displaying number (0-9) */
{
  switch(a)
  {
    case 0:
    digitalWrite(disp_pin[0], LOW);	/* Drive disp_pin[0] to LOW */
    digitalWrite(disp_pin[1], LOW);	/* Driving LOW turns on LED segment for common anode display */
    digitalWrite(disp_pin[2], LOW);
    digitalWrite(disp_pin[3], LOW);
    digitalWrite(disp_pin[4], LOW);
    digitalWrite(disp_pin[5], LOW);
    digitalWrite(disp_pin[6], HIGH); 
    break;
    case 1:
    digitalWrite(disp_pin[0], HIGH);	/* Drive disp_pin[7] to HIGH */
    digitalWrite(disp_pin[1], LOW);
    digitalWrite(disp_pin[2], LOW);
    digitalWrite(disp_pin[3], HIGH);	/* Driving HIGH turns off LED segment for common anode display */
    digitalWrite(disp_pin[4], HIGH);
    digitalWrite(disp_pin[5], HIGH);
    digitalWrite(disp_pin[6], HIGH);
    break;
    case 2:
    digitalWrite(disp_pin[0], LOW);
    digitalWrite(disp_pin[1], LOW);
    digitalWrite(disp_pin[2], HIGH);
    digitalWrite(disp_pin[3], LOW);
    digitalWrite(disp_pin[4], LOW);
    digitalWrite(disp_pin[5], HIGH);
    digitalWrite(disp_pin[6], LOW);
    break;
    case 3:
    digitalWrite(disp_pin[0], LOW);
    digitalWrite(disp_pin[1], LOW);
    digitalWrite(disp_pin[2], LOW);
    digitalWrite(disp_pin[3], LOW);
    digitalWrite(disp_pin[4], HIGH);
    digitalWrite(disp_pin[5], HIGH);
    digitalWrite(disp_pin[6], LOW);
    break;
    case 4:
    digitalWrite(disp_pin[0], HIGH);
    digitalWrite(disp_pin[1], LOW);
    digitalWrite(disp_pin[2], LOW);
    digitalWrite(disp_pin[3], HIGH);
    digitalWrite(disp_pin[4], HIGH);
    digitalWrite(disp_pin[5], LOW);
    digitalWrite(disp_pin[6], LOW);
    break;
    case 5:
    digitalWrite(disp_pin[0], LOW);
    digitalWrite(disp_pin[1], HIGH);
    digitalWrite(disp_pin[2], LOW);
    digitalWrite(disp_pin[3], LOW);
    digitalWrite(disp_pin[4], HIGH);
    digitalWrite(disp_pin[5], LOW);
    digitalWrite(disp_pin[6], LOW);
    break;
    case 6:
    digitalWrite(disp_pin[0], LOW);
    digitalWrite(disp_pin[1], HIGH);
    digitalWrite(disp_pin[2], LOW);
    digitalWrite(disp_pin[3], LOW);
    digitalWrite(disp_pin[4], LOW);
    digitalWrite(disp_pin[5], LOW);
    digitalWrite(disp_pin[6], LOW);
    break;
    case 7:
    digitalWrite(disp_pin[0], LOW);
    digitalWrite(disp_pin[1], LOW);
    digitalWrite(disp_pin[2], LOW);
    digitalWrite(disp_pin[3], HIGH);
    digitalWrite(disp_pin[4], HIGH);
    digitalWrite(disp_pin[5], HIGH);
    digitalWrite(disp_pin[6], HIGH);
    break;
    case 8:
    digitalWrite(disp_pin[0], LOW);
    digitalWrite(disp_pin[1], LOW);
    digitalWrite(disp_pin[2], LOW);
    digitalWrite(disp_pin[3], LOW);
    digitalWrite(disp_pin[4], LOW);
    digitalWrite(disp_pin[5], LOW);
    digitalWrite(disp_pin[6], LOW);
    break;
    case 9:
    digitalWrite(disp_pin[0], LOW);
    digitalWrite(disp_pin[1], LOW);
    digitalWrite(disp_pin[2], LOW);
    digitalWrite(disp_pin[3], LOW);
    digitalWrite(disp_pin[4], HIGH);
    digitalWrite(disp_pin[5], LOW);
    digitalWrite(disp_pin[6], LOW);
    break;
    default:
    digitalWrite(disp_pin[0], HIGH);
    digitalWrite(disp_pin[1], LOW);
    digitalWrite(disp_pin[2], LOW);
    digitalWrite(disp_pin[3], LOW);
    digitalWrite(disp_pin[4], LOW);
    digitalWrite(disp_pin[5], HIGH);
    digitalWrite(disp_pin[6], LOW);
    break;
  }
}

void loop() {
  int i;
  for(i = 9; i>=0; i--)
  {
    sevenSegmentDisplay(i);
    delayUs(1000);
  }
  for(i = 0; i<=9; i++)
  {
    sevenSegmentDisplay(i);
    delayUs(1000);
  }
}


