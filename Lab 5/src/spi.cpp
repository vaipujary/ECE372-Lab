// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Assignment:  Lab 5
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//

#include "spi.h"
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define wait_for_completion while(!(SPSR & (1<<SPIF)));

void initSPI(){
}