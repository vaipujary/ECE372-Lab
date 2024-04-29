// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Project:     RFID Snack Dispenser
//
// Description: This file handles the operation of the motor that dispenses the snacks.

//----------------------------------------------------------------------//

#include <avr/io.h>
#include "pwm.h"
#include "timer.h"

// One duty cycle, two PWM signals
void changeDutyCycle(unsigned int OCR_num)
{ // Pass in the desired OCR value. 1023 for max output, 0 for no output.
    // OCR_num is a 16 bit number that contains the 10-bit combination of ADCH and ADCL registers.
    // Obtain the correct speed and direction on the DC motor.

    // Clockwise rotation of motor
    if (OCR_num != 512)
    {
        // 0 represents max ramp (100% duty cycle,) and 512 represents min rmp (0% duty cycle)
        OCR3A = (1024 - OCR_num);
        OCR4A = OCR_num;
    }
    // Motor not moving
    else
    {
        OCR3A = 0;
        OCR4A = 0;
    } // Calculate OCR3A based on PWM period. Hardcoded for now, later change to use ADC registers
    // 1023*duty cyle amount
}

void initPWMTimer3()
{
    // Timer 3A, on pin 5 on the board
    // Data direction pins
    DDRE |= (1 << DDE3);

    // Set pins for non-inverting timer
    TCCR3A |= (1 << COM3A1) | (1 << WGM31);
    TCCR3A &= ~(1 << COM3A0) | (1 << WGM30);
    TCCR3B |= (1 << WGM32) | (1 << CS30) | (1 << WGM33);
    TCCR3B |= ~(1 << CS31);
    TCCR3B &= ~(1 << CS32);
    // changeDutyCycle(0); // default PWM to off
    OCR3A = 0;
    ICR3 = 1024;
}

void initPWMTimer4()
{
    // Set header pin 6 to output, corresponding to OC4A
    // Data direction pins
    DDRH |= (1 << DDH3);
    // set non-inverting mode - output starts high and then is low,
    // COM1A0 bit = 1
    // COM1A1 bit = 1
    TCCR4A &= ~(1 << COM4A0);
    TCCR4A |= (1 << COM4A1);

    //  Use  PWM mode 10 bit, top value is determined by ICR1 value,
    //  which determines the PWM frequency.
    // for mode 14:
    // WGM40 = 0
    // WGM41 = 1
    // WGM42 = 1
    // WGM43 = 1
    TCCR4A &= ~(1 << WGM40);
    TCCR4A |= (1 << WGM41);
    TCCR4B |= (1 << WGM42);
    TCCR4B |= (1 << WGM43);

    ICR4 = 1023;

    // set prescalar CSBits to prescaler of 1
    // CS30 =1
    // CS31 =0
    // CS32 =0
    TCCR4B |= (1 << CS41);
    TCCR4B &= ~((1 << CS40) | (1 << CS42));

    // The last thing is to set the duty cycle.
    OCR4A = 0;
}