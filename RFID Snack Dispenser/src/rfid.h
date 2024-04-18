#ifndef RFID_H
#define RFID_H

#include <mfrc522.h>

extern MFRC522 mfrc522;

void init_rfid();
int read_rfid();

#endif
