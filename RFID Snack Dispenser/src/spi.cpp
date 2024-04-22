// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Project:     RFID Snack Dispenser
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//

#include <avr/interrupt.h>
#include <avr/io.h>
#include "spi.h"

#define DDR_SPI DDRB      // Data Direction Register on ATMEGA2560 for SPI is DDRB
#define DD_SS DDB0        // SS Chip Select data direction bit B0 of ATMEGA2560 is DDB0
#define DD_SCK DDB1       // Clock pin connection data direction bit B1 on ATMEGA2560 is DDB1
#define DD_MOSI DDB2      // MOSI pin datadirection on ATMEGA2560 is DDB2
#define DD_MISO DDB3      // MISO pin datadirection on ATMEGA2560 is DDB3
#define SPI_PORT PORTB    // PortB for SPI on ATMEGA2560 is PORTB
#define SPI_SS_BIT PORTB0 // Port B register Bit B0 of Chip Select on ATMEGA2560 is PORTB0

#define wait_for_complete         \
    while (!(SPSR & (1 << SPIF))) \
        ;

void initSPI()
{
    // set MOSI,SCK,and SS direction to outputs
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_SS);

    // set MISO direction to input
    DDR_SPI &= ~(1 << DD_MISO);

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
    wait_for_complete;              // wait for flag to raise
    SPDR = data;                    // load the data into register
    wait_for_complete;              // wait for flag to raise
    SPI_PORT |= (1 << SPI_SS_BIT);  // disable chip select to end SPI frame
}

void write_execute_bytes(unsigned char CMD, unsigned char count, unsigned char *values)
{

    SPI_PORT &= ~(1 << SPI_SS_BIT); // enable chip select bit to begin SPI frame
    SPDR = CMD;                     // load the CMD address into register
    wait_for_complete;              // wait for flag to raise
    for (unsigned char i = 0; i < count; i++)
    {
        SPDR = values[i];  // load the data into register
        wait_for_complete; // wait for flag to raise
    }
    SPI_PORT |= (1 << SPI_SS_BIT); // disable chip select to end SPI frame
}

unsigned char read_execute(unsigned char reg_address)
{
    write_execute(0x80 | reg_address, 0x00); // 0x80 | reg_address is the internal Register Address with the write bit masked on (0x80)
                                             // 0x00 is 'dummy' data
    return SPDR;
}