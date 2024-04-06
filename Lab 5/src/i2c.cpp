// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Assignment:  Lab 5
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//
// TWCR - TWI Control Register
// TWINT: Interrupt flag (clear flag by setting bit to 1)
// TWEA: Enable acknowledge
// TWSTA: Start condition
// TWSTO: Stop condition
// TWWC: Write collision flag
// TWEN: I2C enable (enables the I2C module)
// TWIE: Interrupt enable

#include "i2c.h"
#include <avr/io.h>
#include "Arduino.h"
#define wait_for_completion        \
    while (!(TWCR & (1 << TWINT))) \
        ;

void initI2C()
{
    // Wake up I2C module from power reduction mode
    PRR0 &= ~(1 << PRTWI); // Page 236 of ATMEGA 2560 datasheet

    // Generate a 10kHz clock rate
    // SCL Frequency = 16 MHz/(16+2(TWBR)*4)
    // Table 24-7 prescaler value = 1
    TWSR |= (1 << TWPS0);
    TWSR &= ~(1 << TWPS1);

    // Bit rate generator = 10k (TWBR = 198)
    TWBR = 0xC6;

    // Enable two wire interface
    TWCR |= (1 << TWINT) | (1 << TWEN);
}

// Function to send a start condition to start the I2C transmission
void startI2C_Trans(unsigned char I2C_SLA)
{
    // Set the start condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // reset flag TWINT, intiate a start condition and enable

    // Wait flag to check read/write has completed
    wait_for_completion;

    // Send the data, i.e., slave address + write bit '0'
    TWDR = (I2C_SLA << 1) & (0xFE); // Now has 7 bits

    // Trigger I2C action
    TWCR = (1 << TWINT) | (1 << TWEN);

    wait_for_completion;
}

// Function to send a stop condition to stop the I2C transmission
void stopI2C_Trans()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Trigger action
}

// Function to load and transmit the data passed into the I2C data register
void write(unsigned char dataToSend)
{
    TWDR = dataToSend;                 // Load data into TWDR register
    TWCR = (1 << TWINT) | (1 << TWEN); // Trigger I2C action

    wait_for_completion;
}

// Function to read 1 byte of data from SLA at the MEMADDRESS
void read_From(unsigned char I2C_SLA, unsigned char MEMADDRESS)
{
    startI2C_Trans(I2C_SLA);

    // Write the address of the register in the slave device
    write(MEMADDRESS);

    // Perform repeated start
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    wait_for_completion;

    // Send 7 bit slave address plus read bit (SLA + R)
    TWDR = (I2C_SLA << 1) | (0x01);

    // Trigger I2C action with acknowledge bit sent down the line
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

    wait_for_completion;

    // No acknowledge bit (NACK/NOT ACK) set by master
    TWCR = (1 << TWINT) | (1 << TWEN); // Trigger I2C action

    wait_for_completion;

    // Set the stop condition
    stopI2C_Trans();

    // TWDR register now has the data from SLA that master reads
}

// Function to return the last byte from the data register
unsigned char read_Data()
{
    return TWDR;
}
