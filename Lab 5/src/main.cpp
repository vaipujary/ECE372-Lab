// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Assignment:  Lab 5
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//

#include <Arduino.h>
#include <avr/io.h>
#include "spi.h"
#include "pwm.h"
#include "timer.h"
#include "switch.h"
#include "i2c.h"
typedef enum {
  waitPress,
  debouncePress,
  waitRelease,
  debounceRelease} buttonState;
typedef enum {
    LEDSMILLEY,LEDSAD
}LEDFACES;
int ChirpOn=0;//chirp=0 no chirp,1 chirping
int main(){
    initI2C();
    initPWMTimer3();
    initSPI();
    initTimer0();

    while(1) {
      switch (LEDFACES)
      {
      case LEDSMILLEY:
        
        break;
      case LEDSAD:
        break;
      default:
        break;
      }
      switch (buttonState)
      {
      case waitPress:
        /* code */
        break;
      case debouncePress:
        break;
      case waitRelease:
        break;
      case debounceRelease:
        break;
      default:
        break;
      }
    }
}


// ISR(INT2_vect) {

// }