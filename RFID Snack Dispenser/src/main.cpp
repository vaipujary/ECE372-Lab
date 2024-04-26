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
#include "led.h"
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

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

int rfidUID = 0; // Variable to store RFID UID

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
    initSwitchPD2();    // Initialize switch
    initTimer1();       // Initialize timers
    initTimer0();
    initPWMTimer3();
    initLCD();        // Initialize LCD
    initLED();        // Initialize LEDs
    moveCursor(0, 0); // moves the cursor to 0,0 position
    writeString("Snack Dispenser");

    while (1)
    {
        rfidUID = readRFID();
        Serial.print("The UID is: ");
        Serial.println(rfidUID);

        // Button state machine logic
        switch (myButtonState)
        {
        ///////////////////////////////Press States/////////////////////////////////////////
        case waitPress: // the "natural" state
            // Do nothing, wait for button to be pressed
            Serial.println("waitPress");
            break;

        case debouncePress: // Debounce Press state, wait for switch debounce state to end
            Serial.println("debouncePress");
            delayMs(1);
            myButtonState = waitRelease;
            break;

        ///////////////////////////////Release States///////////////////////////////
        case waitRelease: // waits for button to be released after pressed
            Serial.println("waitRelease");
            break;

        case debounceRelease:
            Serial.println("debounceRelease");
            delayMs(1);
            myButtonState = waitPress;
            break;

        default:
            break;
        }

        // Normal operation mode
        if (operationMode == normal) // Dispense snacks, turn green LEDs on, display success message on lcd
        {
            Serial.println("Normal operation mode");
            turnOnGreenLED();
            turnOffRedLED();
            if (isAuthorized(rfidUID))
            {
                // Dispense the snacks. Motor moves counterclockwise by default
                Serial.println("RFID UID Authorized!");

                for (int i = 0; i < 512; i++)
                {
                    changeDutyCycle(0);
                }
                moveCursor(1, 0); // moves the cursor to 0,0 position
                writeString("Enjoy the snacks!");
                rfidUID = readRFID(); // Get updated reading of UID
                changeDutyCycle(512);
            }
        }

        // Emergency operation mode
        else // Someone is stealing snacks. Stop motor, turn red LEDs on, display error message on lcd
        {
            Serial.println("!!!Emergency operation mode!!!");
            turnOnRedLED();
            turnOffGreenLED();
            changeDutyCycle(512);
            delayMs(1000);
            moveCursor(1, 0); // moves the cursor to 0,0 position
            writeString("Snack thief!!!!!");
        }
    }
}

// Interrupt Service Routine
ISR(INT2_vect)
{
    Serial.println("Button pressed!");

    if (myButtonState == waitPress)
    {
        Serial.println("waitPress --> debouncePress");
        if (operationMode == normal)
        {
            Serial.println("ENTERING EMERGENCY MODE");
            operationMode = emergency;
        }

        else
        {
            Serial.println("NORMAL MODE");
            operationMode = normal;
        }

        myButtonState = debouncePress;
    }
    else if (myButtonState == waitRelease)
    {
        Serial.println("waitRelease --> debounceRelease");

        if (operationMode == normal)
        {
            Serial.println("ENTERING EMERGENCY MODE");
            operationMode = emergency;
        }

        else
        {
            Serial.println("NORMAL MODE");
            operationMode = normal;
        }
        myButtonState = debounceRelease;
    }
}