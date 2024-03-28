// Authors:     Group 3
// Date:        04/04/2024
// Authors:     Group 3
// Date:        04/04/2024
// Assignment:  Lab 4
//
// Description:
//
// Requirements:
//      3.There are no non-trivial SFR manipulations in the main function and
//      are wrapped in functions that have meaningful names
//      4.A state machine is used to implement the bulk of the functionality
//      of the program
//----------------------------------------------------------------------//

#include <Arduino.h>
#include <avr/io.h>
#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"
#include "sevensegmentdisplay.h"
#include <avr/interrupt.h>

// A state machine is used to implement the bulk of the functionality of the program
typedef enum
{
  waitPress,
  debouncePress,
  waitRelease,
  debounceRelease,
  alarm
} buttonState; // Define a set of states that can be used in the state machine using an enum.

volatile buttonState myButtonState = waitPress;
volatile int flip = 1;
volatile int i = 9;
int main()
{
  initADC();
  initTimer1();
  initTimer0();
  initSevenSegment();
  initPWMTimer3();
  initPWMTimer4();
  initSwitchPD0();
  sei(); // Enable global interrupts.

  // while loop
  while (1)
  {
    if (flip == 1)
    { // clockwise
      changeDutyCycle(255);
    // voltage for potentionmeter=5V 
      // writeString("Clockwise");
    }
    else if (flip == 2)
    { // counterclockwise
      //voltage for potentionmeter=0V
      // writeString("CCwise");
    }

    // State machine logic
    switch (myButtonState)
    {

    case waitPress: // the "natural" state

      break;

    case debouncePress:
      i = 9;
      while (i >= 0)
      {
        sevenSegmentDisplay(i);
        delayMs(1000);
        i = i - 1;
      }
      myButtonState = waitRelease;
      break;

    case waitRelease: // waits for button to be released after pressed
      myButtonState = debounceRelease;
      break;

    case debounceRelease:
      myButtonState = waitPress;
      break;

    case alarm:

      myButtonState = waitPress;
      break;

    default:
      break;
    }
  }
  return 0;
}
ISR(PCINT0_vect)
{
  if (myButtonState == waitPress)
  {
    myButtonState = debouncePress;
  }
  else if (myButtonState == waitRelease)
  {

    if (flip == 1)
    {
      flip = 2;
    }
    else
    {
      flip = 1;
    }
    myButtonState = debounceRelease;
  }
}