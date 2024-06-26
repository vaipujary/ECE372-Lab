// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Project:     RFID Snack Dispenser
//
// Description: This file handles the initialization of the RFID, parsing the UID of the RFID tag/card, and checking
// if a certain UID is authorized.
//----------------------------------------------------------------------//

#include "rfid.h"
#include "spi.h"
#define UID_LIST_SIZE 3

int Authorized_UID_list[UID_LIST_SIZE] = {609, 359};
// Vaidehi card: 609, tag: 359
// Logan card: 638, tag: 353

void initRFID()
{
    mfrc522.PCD_Init();                // Init MFRC522
    delayMs(0.5);                      // Optional delay. Some board do need more time after init to be ready
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
}

// Function to read in an RFID card/tag's unique identification number
int readRFID()
{
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        // Serial.println("New card not present");
        return 0;
    }

    // Select one of the cards
    else if (!mfrc522.PICC_ReadCardSerial())
    {
        // Serial.println("Cannot read card");
        return 0;
    }

    // UID extraction: converting UID bytes to an integer for readability and ease of manipulation
    int num = 0;

    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        num = num + mfrc522.uid.uidByte[i];
    }

    return num;
}

// Function to check if the user's RFID UID is authorized
bool isAuthorized(int rfidUID)
{
    for (int i = 0; i < UID_LIST_SIZE; i++)
    {
        if (rfidUID == Authorized_UID_list[i] && rfidUID != 0)
        {
            return true;
        }
    }
    return false;
}