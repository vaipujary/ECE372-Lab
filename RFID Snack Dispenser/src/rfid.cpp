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

void init_rfid()
{

    // SPI.begin();			// Init SPI bus
    mfrc522.PCD_Init();                // Init MFRC522
    delayMs(4);                        // Optional delay. Some board do need more time after init to be ready, see Readme
    mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
}

int read_rfid()
{

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return 0;
    }

    // Select one of the cards
    else if (!mfrc522.PICC_ReadCardSerial())
    {
        return 0;
    }

    // Dump debug info about the card; PICC_HaltA() is automatically called
    // UID
    int num = 0;
    // Serial.print(F("Card UID:"));
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        num = num + mfrc522.uid.uidByte[i];
    }
    // Serial.println(num);
    return (num);
}