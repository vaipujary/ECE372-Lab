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

// Global variables and macros
#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 53 // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

int rfidUID = 0;
int UID_list[10];

typedef enum
{
    waitPress,
    debouncePress,
    waitRelease,
    debounceRelease
} buttonState;

volatile buttonState myButtonState = waitPress;

// Main function
int main(void)
{
    sei();              // Enable global interrupts
    Serial.begin(9600); // Initialize serial port
    initSPI();
    initRFID();
    initSwitchPD0();
    rfidUID = readRFID();

    while (1)
    {

        // Button state machine logic
        switch (myButtonState)
        {
            ///////////////////////////////Press States/////////////////////////////////////////
        case waitPress: // the "natural" state
            // Do nothing, wait for button to be pressed
            Serial.println(1);
            break;

        case debouncePress: // Debounce Press state, wait for swithc debounce state to end
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
    }
}