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
volatile int x;
volatile int y;
volatile int z;

#define MPU_WHO_AM_I 0x68
volatile buttonState myButtonState = waitPress;
volatile LEDFACES LEDState =LEDSMILLEY;
int ChirpOn=0;//chirp=0 no chirp,1 chirping
int main(){
    initI2C();
    initPWMTimer3();
    initSPI();
    initTimer1();
    initSwitchPD2();
    Serial.begin(9600);
    read_Data();
    while(1) {
      Serial.print("x: "+String(x)+"\n");
      Serial.print("y: "+String(y)+"\n");
      Serial.print("z: "+String(z)+"\n");
      for (int i = 1000; i <= 4000; i++) {
        changeFrequency(i);
      }

      switch (LEDState)
      {
      case LEDSMILLEY:
        displaySmile();
        alarmOff();
        break;
      case LEDSAD:
        displayFrown();
        break;
      default:
        break;
      }
      switch (myButtonState)
      {
      case waitPress:
        Serial.println("waitPress");
        delayMs(1000);
        break;
      case debouncePress:
        Serial.println("debouncePress");
        myButtonState = waitRelease;
        break;
      case waitRelease:
        Serial.println("waitRelease");
        break;
      case debounceRelease:
        Serial.println("debounceRelease");
        myButtonState = waitPress;
        break;
      default:
        break;
      }
    }
}


ISR(INT2_vect) {
  
  if(x>10 && y>10 && z>10){
    alarmOn();
    LEDState=LEDSAD;
  }
  if (myButtonState == waitPress)
  {
    Serial.println("debouncePress");
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
    LEDState=LEDSMILLEY;
  }


}