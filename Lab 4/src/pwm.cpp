#include <avr/io.h>

#include "pwm.h"

// One duty cycle
void changeDutyCycle(unsigned int OCR_num)
{ // Pass in the desired OCR value. 1023 for max output, 0 for no output.
    // OCR_num is a 16 bit number that contains the 10-bit combination of ADCH and ADCL registers.
    // Obtain the correct speed and direction on the DC motor.

    // Clockwise rotation of motor
    if (OCR_num < 512)
    {
        // 0 represents max ramp (100% duty cycle,) and 512 represents min rmp (0% duty cycle)
        OCR3A = 2 * (0b1111111111 - OCR_num);
        OCR4A = 0;
    }
    // Counterclockwise rotation of motor
    else if (OCR_num > 512)
    {
        // 0 represents max ramp (100% duty cycle,) and 512 represents min rmp (0% duty cycle)
        OCR4A = 2 * (OCR_num);
        OCR3A = 0;
    }
    // Motor not moving
    else
    {
        OCR3A = (0);
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
    TCCR3A |= (1 << COM3A1) | (1 << WGM31) | (1 << WGM30);
    TCCR3A &= ~(1 << COM3A0);
    TCCR3B |= (1 << WGM32) | (1 << CS30);
    TCCR3B &= ~(1 << CS31);
    TCCR3B &= ~(1 << CS32);
    // changeDutyCycle(0); // default PWM to off
    OCR3A = 512;
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
    TCCR4A |= (1 << WGM40);
    TCCR4A |= (1 << WGM41);
    TCCR4B |= (1 << WGM42);
    TCCR4B &= ~(1 << WGM43);

    ICR4 = 1023;

    // set prescalar CSBits to prescaler of 1
    // CS30 =1
    // CS31 =0
    // CS32 =0
    TCCR4B |= (1 << CS40);
    TCCR4B &= ~((1 << CS41) | (1 << CS42));

    // The last thing is to set the duty cycle.
    OCR4A = 1023;
}

// Two duty cycles
// // Function that changes the duty cycle of the motor
// // Takes in argument that is a combination of the analog to digital conversion, ADCH and ADCL registers
// void changeDutyCycle(double dutyCycle1, double dutyCycle2)
// { // Pass in the desired PWM signals. 1023 for max output, 0 for no output.

//     OCR3A = int(dutyCycle1 * 1024); // Calculate OCR3A based on duty cycle.
//     OCR4A = int(dutyCycle2 * 1024); // Calculate OCR4A based on duty cycle.
// }

// // function that produces an output PWM signal with a frequency and duty cycle. For this example
// // I am generating a PWM frequency of 15.625 kHz with a 60% duty cycle.
// // I will use Timer 1 in the fast PWM mode #7 in Table 17-2, where my TOP value is 0x3FF (1023)
// // In this mode I have a frequency that is fixed by the top value. The only setting that
// // I can change is the prescaler to lower the frequency to a specific value.
// // Use Fast PWM Mode #14 if you need more flexibilty on the frequency value.

// // I will have a prescaler of 1.  The calculation of OCR3A is shown below:
// void initPWMTimer3()
// {
//     // Set header pin 5 to output, corresponding to OC3A
//     DDRE |= (1 << DDE3);
//     // Set non-inverting mode - output starts high and then is low,
//     // COM1A0 bit = 0
//     // COM1A1 bit =1
//     TCCR3A |= (1 << COM3A1);
//     TCCR3A &= ~(1 << COM3A0);

//     //  Use fast PWM mode 10 bit, top value is determined by Table 17-2 of 0x3FF (1023)
//     //  which determines the PWM frequency.
//     // for Fast PWM 10bit mode # 14:
//     // WGM10 = 0
//     // WGM11 = 1
//     // WGM12 = 1
//     // WGM13 = 1
//     TCCR3A &= ~(1 << WGM30);
//     TCCR3A |= (1 << WGM31);
//     TCCR3B |= (1 << WGM32);
//     TCCR3B |= (1 << WGM33);

//     // PWM frequency calculation for FAST PWM mode on page 148 of datasheet
//     // Frequency of PWM = (F_clk)/((Prescaler)* (1 +TOP))
//     // Frequency of PWM = 16Mhz
//     // Prescaler = 1
//     // TOP value = 0x3FF = 1023
//     // PWM frequency from calculation = 15.625 kHz

//     ICR3 = 1023;

//     // Set prescalar CSBits to prescaler of 1
//     //  CS10 =1
//     //  CS11 =0
//     //  CS12 =0
//     TCCR3B |= (1 << CS31);
//     TCCR3B &= ~((1 << CS30) | (1 << CS32));

//     // The last thing is to set the duty cycle.
//     // duty cycle is set by dividing output compare OCR3A value by 1 + TOP value
//     // the top value is (1 + ICR3) = 1024
//     // calculate OCR3A value => OCR3A = duty cycle(fractional number) * (1 + TOP)
//     // we want a duty cycle = 60%
//     // OCR1A = 0.60 * 1024
//     OCR3A = 614;
// }

// // I will have a prescaler of 1.  The calculation of OCR1A are shown below:
// void initPWMTimer4()
// {
//     // Set header pin 6 to output, corresponding to OC4A
//     DDRH |= (1 << DDH3);
//     // set non-inverting mode - output starts high and then is low,
//     // COM1A0 bit = 1
//     // COM1A1 bit = 1
//     TCCR4A |= (1 << COM4A0);
//     TCCR4A |= (1 << COM4A1);

//     //  Use  PWM mode 10 bit, top value is determined by ICR1 value,
//     //  which determines the PWM frequency.
//     // for mode 14:
//     // WGM40 = 0
//     // WGM41 = 1
//     // WGM42 = 1
//     // WGM43 = 1
//     TCCR4A &= ~(1 << WGM40);
//     TCCR4A |= (1 << WGM41);
//     TCCR4B |= (1 << WGM42);
//     TCCR4B |= (1 << WGM43);

//     ICR4 = 1023;

//     // set prescalar CSBits to prescaler of 1
//     // CS30 =1
//     // CS31 =0
//     // CS32 =0
//     TCCR4B |= (1 << CS41);
//     TCCR4B &= ~((1 << CS40) | (1 << CS42));

//     // The last thing is to set the duty cycle.
//     OCR4A = 1023;
// }
