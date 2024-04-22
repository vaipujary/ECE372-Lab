// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Project:     RFID Snack Dispenser
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//
#include <MFRC522.h>
#include "rfid.h"
#include "spi.h"
#include "timer.h"
#include "pwm.h"
#include "switch.h"
#include <stdio.h>

/*
 * Pin layout should be as follows for the MFRC522 board:
 * Signal     Pin              Pin
 *            Arduino Mega     MFRC522 board
 * -----------------------------------------------
 * Reset      46               RST
 * SPI SS     53               SDA
 * SPI MOSI   51               MOSI
 * SPI MISO   50               MISO
 * SPI SCK    52               SCK
 */

// Global variables and macros
#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 53 // Configurable, see typical pin layout above
#define UID_LIST_SIZE 10

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

int rfidUID = 0;
int UID_list[UID_LIST_SIZE];

typedef enum
{
    waitPress,
    debouncePress,
    waitRelease,
    debounceRelease,
    normal,
    emergency
} state;

volatile state myButtonState = waitPress;
volatile state operationMode = normal;

// Main function
int main(void)
{
    sei();              // Enable global interrupts
    Serial.begin(9600); // Initialize serial port
    initSPI();          // Initialize SPI communication
    initRFID();         // Initialize RFID module
    initSwitchPD0();    // Initialize switch
    initTimer1();       // Initialize timer

    rfidUID = readRFID();

    while (1)
    {

        // Button state machine logic
        switch (myButtonState)
        {
            ///////////////////////////////Press States/////////////////////////////////////////
        case waitPress: // the "natural" state
            // Do nothing, wait for button to be pressed
            Serial.println("waitPress");
            Serial.println(1);
            break;

        case debouncePress: // Debounce Press state, wait for switch debounce state to end
            Serial.println("debouncePress");
            Serial.flush();
            delayMs(1);
            myButtonState = waitRelease;
            break;

        ///////////////////////////////Release States///////////////////////////////
        case waitRelease: // waits for button to be released after pressed
            Serial.println("waitRelease");
            Serial.flush();
            delayMs(1);

            break;

        case debounceRelease:
            Serial.println("debounceRelease");
            Serial.flush();
            delayMs(1);
            myButtonState = waitPress;
            break;

        default:
            break;
        }

        if (operationMode == normal) // Dispense snacks, turn LEDs green, display success message on lcd
        {
            for (int i = 0; i < UID_LIST_SIZE; i++)
            {
                // User is authorized, activate the motor
                if (rfidUID == UID_list[i])
                {
                    Serial.println('RFID UID Authorized');
                    motorCCW(); // Dispense the snacks
                    delayMs(5000);
                }
            }
        }
        else // Emergency operation mode. Someone is stealing snacks. Stop motor, turn LEDs red, display error message on lcd
        {
        }
    }
}

// Interrupt Service Routine
ISR(PCINT0_vect)
{
    Serial.println("Button pressed!");

    if (myButtonState == waitPress)
    {
        Serial.println("waitPress --> debouncePress");
        myButtonState = debouncePress;
    }
    else if (myButtonState == waitRelease)
    {
        Serial.println("waitRelease --> debounceRelease");
        myButtonState = debounceRelease;

        if (operationMode == normal)
        {
            operationMode = emergency;
        }

        else
        {
            operationMode = normal;
        }
    }
}