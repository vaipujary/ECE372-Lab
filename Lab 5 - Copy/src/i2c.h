#ifndef I2C_H
#define I2C_H

void initI2C();
void startI2C_Trans(unsigned char I2C_SLA);
void stopI2C_Trans();
void write(unsigned char dataToSend);
void read_From(unsigned char I2C_SLA, unsigned char MEMADDRESS);
unsigned char read_Data();

#endif