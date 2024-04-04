// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
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
  debounceRelease
} buttonState; // Define a set of states that can be used in the state machine using an enum.

volatile buttonState myButtonState = waitPress;
volatile int flip = 1;
volatile int i = 9;

int main()
{
  Serial.begin(9600);
  // Initializations
  initADC();
  initTimer1();
  initTimer0();
  initSevenSegment();
  initPWMTimer3();
  initPWMTimer4();
  initSwitchPD0();
  sei(); // Enable global interrupts.

  unsigned int result = 0;
  // while loop
  while (1)
  {
    turnOnImsk(); // Enable INT0 in the EIMSK register
    result = ADCL;
    result += ((unsigned int)ADCH) << 8;
    changeDutyCycle(result);
    // Serial.println(result);
    // Serial.println(result);

    // if (flip == 1)
    // {                       // clockwise
    //   changeDutyCycle(768); //.75% duty cycle
    //   // voltage for potentionmeter=5V
    //   // writeString("Clockwise");
    // }
    // else if (flip == 2)
    // { // counterclockwise
    //   // voltage for potentionmeter=0V
    //   changeDutyCycle(255); //.25% duty cycle
    //   // writeString("CCwise");
    // }
    // if (flip == 1)
    // {                       // clockwise
    //   changeDutyCycle(768); //.75% duty cycle
    //   // voltage for potentionmeter=5V
    //   // writeString("Clockwise");
    // }
    // else if (flip == 2)
    // { // counterclockwise
    //   // voltage for potentionmeter=0V
    //   changeDutyCycle(255); //.25% duty cycle
    //   // writeString("CCwise");
    // }

    // State machine logic
    switch (myButtonState)
    {
      ///////////////////////////////Press States/////////////////////////////////////////
    case waitPress: // the "natural" state
      // Do nothing, wait for button to be pressed
      Serial.println("waitPress");
      break;

    case debouncePress: // Debounce Press state, wait for switch debounce state to end
      delayMs(1);
      Serial.println("debouncePress");
      myButtonState = waitRelease;
      break;

    ///////////////////////////////Release States///////////////////////////////
    case waitRelease: // waits for button to be released after pressed
      Serial.println("waitRelease");
      break;

    case debounceRelease:
      Serial.println("debounceRelease");
      turnOffImsk(); // Disable INT0 in the EIMSK register

      for (int i = 9; i >= 0; i--)
      {
        changeDutyCycle(512);
        sevenSegmentDisplay(i);
        delayMs(1000);
      }
      sevenSegmentDisplay(0);

      // Enable the button interrupt
      turnOnImsk(); // Enable INT0 in the EIMSK register

      // Wait for the noisy 'debounce' state to pass. Then, we are awaiting press.
      delayMs(1);

      myButtonState = waitPress;
      break;

    default:
      myButtonState=waitPress;
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

    // if (flip == 1)
    // {
    //   flip = 2;
    // }
    // else
    // {
    //   flip = 1;
    // }
    myButtonState = debounceRelease;
  }
}