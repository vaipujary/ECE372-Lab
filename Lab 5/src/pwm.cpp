// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Assignment:  Lab 5
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//
#include <avr/io.h>
#include "pwm.h"
#include "timer.h"



void initPWMTimer3()
{
    // Timer 3A, on pin 3 on the board
    // Data direction pins
    DDRE |= (1 << DDE5);

    // Set pins for non-inverting timer
    TCCR3A |= (1 << COM3C1);
    TCCR3A &= ~(1 << COM3C0);

    // Mode 15
    TCCR3A |= (1 << WGM30);
    TCCR3A |= (1 << WGM31); 
    TCCR3B |= (1 << WGM32);
    TCCR3B |= (1 << WGM33); 

    TCCR3B |=  (1 << CS30);
    TCCR3B |= (1 << CS31);
    TCCR3B &= ~(1 << CS32);
}

// Changes frequency of the piezo buzzer
void changeFrequency() {

}

// Turns chirping sound on
void alarmOn(){
  TCCR3B |= (1 << CS30);
  TCCR3B &= ~(1 << CS31);
  TCCR3B &= ~(1 << CS32);
}

// Turns chirping sound off
void alarmOff(){
  TCCR3B &= ~(1 << CS30);
}


