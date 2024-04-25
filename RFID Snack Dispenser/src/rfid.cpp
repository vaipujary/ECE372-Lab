// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Project:     RFID Snack Dispenser
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//

#include "rfid.h"
#include "spi.h"
#define UID_LIST_SIZE 2

int Authorized_UID_list[UID_LIST_SIZE] = {609, 359};
// Vaidehi card: 609, tag: 359
// Logan card: 638, tag: 353

void initRFID()
{
    // SPI.begin();			// Init SPI bus
    mfrc522.PCD_Init();                // Init MFRC522
    delayMs(4);                        // Optional delay. Some board do need more time after init to be ready, see Readme
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
}

int readRFID()
{
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        Serial.println("New card not present");
        return 0;
    }

    // Select one of the cards
    else if (!mfrc522.PICC_ReadCardSerial())
    {
        Serial.println("Cannot read card");
        return 0;
    }

    // UID extraction
    int num = 0;
    Serial.print("RFID Function Card UID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        num = num + mfrc522.uid.uidByte[i];
    }
    Serial.println(num);
    Serial.println("Hello");
    return (num);
}

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