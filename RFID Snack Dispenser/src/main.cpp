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
#include "lcd.h"
#include <stdio.h>

/*
 * Pin layout should be as follows for the MFRC522 board:
 * Signal     Pin              Pin
 *            Arduino Mega     MFRC522 board
 * -----------------------------------------------
 * Reset      49               RST
 * SPI SS     53               SDA
 * SPI MOSI   51               MOSI
 * SPI MISO   50               MISO
 * SPI SCK    52               SCK
 */

// Global variables and macros
#define RST_PIN 49 // Configurable, see typical pin layout above
#define SS_PIN 53  // Configurable, see typical pin layout above
#define UID_LIST_SIZE 10

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

int rfidUID = 0;
int Authorized_UID_list[UID_LIST_SIZE] = {609, 359};
// Vaidehi card: 609, tag: 359
// Logan card: 638, tag: 353

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
    // initSwitchPD0();    // Initialize switch
    // initTimer1();       // Initialize timer
    initTimer0();
    // moveCursor(0, 0); // moves the cursor to 0,0 position
    // writeString("Snack Dispenser");
    rfidUID = readRFID();
    Serial.print("Main function The UID is: ");
    Serial.println(rfidUID);

    while (1)
    {

        // Button state machine logic
        // switch (myButtonState)
        // {
        //     ///////////////////////////////Press States/////////////////////////////////////////
        // case waitPress: // the "natural" state
        //     // Do nothing, wait for button to be pressed
        //     Serial.println("waitPress");
        //     break;

        // case debouncePress: // Debounce Press state, wait for switch debounce state to end
        //     Serial.println("debouncePress");
        //     Serial.flush();
        //     delayMs(1);
        //     myButtonState = waitRelease;
        //     break;

        // ///////////////////////////////Release States///////////////////////////////
        // case waitRelease: // waits for button to be released after pressed
        //     Serial.println("waitRelease");
        //     Serial.flush();
        //     delayMs(1);

        //     break;

        // case debounceRelease:
        //     Serial.println("debounceRelease");
        //     Serial.flush();
        //     delayMs(1);
        //     myButtonState = waitPress;
        //     break;

        // default:
        //     break;
        // }

        if (operationMode == normal) // Dispense snacks, turn green LEDs on, display success message on lcd
        {
            for (int i = 0; i < UID_LIST_SIZE; i++)
            {
                // User is authorized, activate the motor
                if (rfidUID == Authorized_UID_list[i] && rfidUID != 0)
                {
                    Serial.println("RFID UID Authorized!");

                    motorCCW(); // Dispense the snacks. // Have to adjust direction of rotation based on experimentation.
                    // moveCursor(0, 0); // moves the cursor to 0,0 position
                    // writeString("Enjoy the snacks!");
                    delayMs(1000);
                }
            }
        }
        else // Emergency operation mode. Someone is stealing snacks. Stop motor, turn red LEDs on, display error message on lcd
        {
            moveCursor(0, 0); // moves the cursor to 0,0 position
            writeString("Snack thief alert!");
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