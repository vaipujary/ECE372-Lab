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
typedef enum {waitPress, debouncePress, waitRelease, debounceRelease, alarm} buttonState; // Define a set of states that can be used in the state machine using an enum.

  volatile buttonState myButtonState = waitPress;

  int main(){
    initADC();
    initTimer1();
    initTimer0();
    initSevenSegment();
    initPWMTimer3();
    initPWMTimer4();
    sei(); // Enable global interrupts.
    

  // while loop
    while (1) { // An infinite while loop must be present.

      // State machine logic
      switch (myButtonState) {

        case waitPress:    // the "natural" state
        break;

        case debouncePress:
        break;

        case waitRelease: //waits for button to be released after pressed
        break;

        case debounceRelease:      
        break;
        
        case alarm:
        break;

        default:
        break;

      }
    }
    return 0;
  }
void countdown(){
  int i=9;
  while(i>=0){
    i=i-1;
  }
}
ISR(PCINT0_vect){
  if(state == wait_press){
    state = debounce_press;
    }
  else if(state == wait_release){
    
    if(flip==1){
      flip=2;
    }
    else{
      flip=1;
    }
    state = debounce_release;
  } 
}