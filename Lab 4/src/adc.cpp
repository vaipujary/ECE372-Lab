// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/04/2024
// Assignment:  Lab 4
//
// Description:
//
// Requirements:
//      1. A function exists to initialize the ADC
//      2. Uses the A0 pin as an input
//----------------------------------------------------------------------//

#include "adc.h"
#include <avr/io.h>

void initADC()
{
    // AVCC = 5V output
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    // Right justified
    ADMUX &= ~(1 << ADLAR);

    // Set ADC0 as single-ended input with MUX[5:0] = 0b000000
    ADMUX &= ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0) | (1 << MUX4) | (1 << MUX3));
    ADCSRB &= ~(1 << MUX5);

    // Use free-running mode ADTS[2:0] = 0b000
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

    // Enable ADC and enable auto-triggering
    ADCSRA |= (1 << ADEN) | (1 << ADATE);

    // Set the ADC clock frequemcy.  Use a pre-scaler of 128
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Enable the analog i/p for ADC pins disable ADC0 pin digital input - pin A0 on board
    DIDR0 |= (1 << ADC0D);

    // Start the first ADC conversion
    ADCSRA |= (1 << ADSC);
}
