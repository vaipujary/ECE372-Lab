// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Project:     RFID Snack Dispenser
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//

#include <avr/io.h>
#include "pwm.h"
#include "timer.h"



void initPWMTimer3() {
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
    
    //256 prescalar
    TCCR3B &= ~(1 << CS30);
    TCCR3B &= ~(1 << CS31);
    TCCR3B |= (1 << CS32);
}

//changes duty cycle depending on direction of movement for the servo motor
void changeDutyCycle(double direction){
    OCR3C = OCR3A * direction;          
}

//
void changeFrequency(unsigned int i) {
    OCR3A = (16000000/i)/256;  
    
}
// Motor moves clockwise
void motorCW(){
    delayMs(5);
    changeDutyCycle(0.1);
}

// Motor moves counterclockwise
void motorCCW(){
    delayMs(5);
    changeDutyCycle(0.05);
}