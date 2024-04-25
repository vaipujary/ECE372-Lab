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

void initPWMTimer3()
{
    // Timer 3A, on pin 3 on the board
    // Data direction pins
    // Set DDR for output
    // Mode 15: OCR3A set as top value
    // OCR1C for output compare
    // Output pin: PE5 (pin 3)
    DDRE |= (1 << DDE5);

    // Set pins for non-inverting timer: COM3C1 = 1, COM3C0 = 0 (located in TCCR3A register)
    // Table 17-3 in datasheet
    TCCR3A |= (1 << COM3C1);
    TCCR3A &= ~(1 << COM3C0);

    // PWM Mode 15 (can vary frequency)
    // WGM13 = 1, WGM12 = 1, WGM11 = 1, WGM10 = 1 (these bits are stored in the TCCR1A and TCCR1B registers)
    TCCR3A |= (1 << WGM30);
    TCCR3A |= (1 << WGM31);
    TCCR3B |= (1 << WGM32);
    TCCR3B |= (1 << WGM33);

    // 256 prescalar value
    // Top value: OCR3A
    // f_PWM = f_CLK / (prescaler * (1 + Top value))
    TCCR3B &= ~(1 << CS30);
    TCCR3B &= ~(1 << CS31);
    TCCR3B |= (1 << CS32);

    // Set duty cycle
    OCR3C = OCR3A * 0.5; // 50% duty cycle
}

// changes duty cycle depending on direction of movement for the servo motor
void changeDutyCycle(double motorDirection)
{
    OCR3C = OCR3A * motorDirection;
}

// Function to change frequency by adjusting the TOP value (OCR3A) and the compare value (OCR3C)
void changeFrequency(unsigned int frequencyValue)
{
    OCR3A = (16000000 / frequencyValue) / 256;
}

// Motor moves clockwise
void motorCW()
{
    delayMs(5);
    changeDutyCycle(0.5);
}

// Motor moves counterclockwise
void motorCCW()
{
    delayMs(5);
    changeDutyCycle(0.5);
}