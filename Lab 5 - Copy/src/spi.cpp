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

#define DDR_SPI DDRB      // Data Direction Register on ATMEGA2560 for SPI is DDRB
#define DD_SS DDB0        // SS Chip Select data direction bit B0 of ATMEGA2560 is DDB0
#define DD_SCK DDB1       // Clock pin connection data direction bit B1 on ATMEGA2560 is DDB1
#define DD_MOSI DDB2      // MOSI pin datadirection on ATMEGA2560 is DDB2
#define SPI_PORT PORTB    // PortB for SPI on ATMEGA2560 is PORTB
#define SPI_SS_BIT PORTB0 // Port B register Bit B0 of Chip Select on ATMEGA2560 is PORTB0
#define wait_for_completion       \
    while (!(SPSR & (1 << SPIF))) \
        ;

void initSPI()
{
    // set MOSI,SCK,and SS direction to outputs
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_SS);

    // note this program does not use MISO line. IT only writes to device

    // set SS high initially (chip select off)
    SPI_PORT |= (1 << SPI_SS_BIT);

    // enable SPI, master mode, CPOL, CPHA, default clock and fosc/128
    // datasheet says sample on rising edge CPOL = 1 CPHA =1
    SPCR |= (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA) | (1 << SPR1) | (1 << SPR0);
}

void write_execute(unsigned char CMD, unsigned char data)
{
    SPI_PORT &= ~(1 << SPI_SS_BIT); // enable chip select bit to begin SPI frame
    SPDR = CMD;                     // load the CMD address into register
    wait_for_completion;            // wait for flag to raise
    SPDR = data;                    // load the data into register
    wait_for_completion;            // wait for flag to raise
    SPI_PORT |= (1 << SPI_SS_BIT);  // disable chip select to end SPI frame
}

// Display a smile on the 8x8 LED matrix
void displaySmile()
{

    write_execute(1, 0b00000000);
    write_execute(2, 0b00100100);
    write_execute(3, 0b00100100);
    write_execute(4, 0b00100100);
    write_execute(5, 0b00000000);
    write_execute(6, 0b01000010);
    write_execute(7, 0b00111100);
    write_execute(8, 0b00000000);
}

// Display a frown on the 8x8 LED matrix
void displayFrown()
{
    write_execute(1, 0b00000000);
    write_execute(2, 0b00100100);
    write_execute(3, 0b00100100);
    write_execute(4, 0b00100100);
    write_execute(5, 0b00000000);
    write_execute(6, 0b00111100);
    write_execute(7, 0b01000010);
    write_execute(8, 0b00000000);
}