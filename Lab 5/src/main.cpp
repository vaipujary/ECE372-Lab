// Authors:     Group 3 Alicia Enriquez, Vaidehi Pujary, Logan Stonehouse, Jake Weithas
// Date:        04/25/2024
// Assignment:  Lab 5
//
// Description:
//
// Requirements:

//----------------------------------------------------------------------//

#include <Arduino.h>
#include <avr/io.h>
#include "spi.h"
#include "pwm.h"
#include "timer.h"
#include "switch.h"
#include "i2c.h"

typedef enum
{
  waitPress,
  debouncePress,
  waitRelease,
  debounceRelease
} buttonState;

typedef enum
{
  LEDSMILEY,
  LEDSAD
} LEDFACES;

volatile int x;
volatile int y;
volatile int z;

// 59 and 60 for x
// 61 and 62 for y
// 63 and 64 for z
#define MPU_XOUT_H 0x3B
#define MPU_XOUT_L 0x3C
#define MPU_YOUT_H 0x3D
#define MPU_YOUT_L 0x3E
#define MPU_ZOUT_H 0x3F
#define MPU_ZOUT_L 0x40
#define MPU_WHO_AM_I 0x68

#define MPU_PWR_MANAGEMENT_1_REG 0x6B
#define MPU_PWR_CONFIG 0x09

#define MPU_PWR_RESET 0x70 // resets all registers in accel to defaults
#define MPU_PWR_MANAGEMENT_2 0x6C

// gyro data
#define MPU_GYRO_XOUT_H 0x43
#define MPU_GYRO_XOUT_L 0x44
#define MPU_GYRO_YOUT_H 0x45
#define MPU_GYRO_YOUT_L 0x46
#define MPU_GYRO_ZOUT_H 0x47
#define MPU_GYRO_ZOUT_L 0x48

volatile buttonState myButtonState = waitPress;
volatile LEDFACES LEDState = LEDSMILEY;
int chirpOn = 0; // chirp=0 no chirp,1 chirping
signed int yGyro = 0;
signed int zGyro = 0;
signed int xGyro = 0;

int main()
{
  initI2C();
  initPWMTimer3();
  initSPI();
  initTimer1();
  initSwitchPD2();
  Serial.begin(9600);

  while (1)
  {
    startI2C_Trans(MPU_WHO_AM_I);
    read_From(MPU_WHO_AM_I, MPU_XOUT_L);
    x = read_Data();

    read_From(MPU_WHO_AM_I, MPU_YOUT_L);
    y = read_Data();
    read_From(MPU_WHO_AM_I, MPU_YOUT_H);
    y = (read_Data() << 8 | y);

    read_From(MPU_WHO_AM_I, MPU_ZOUT_L);
    z = read_Data();
    read_From(MPU_WHO_AM_I, MPU_ZOUT_H);
    z = (read_Data() << 8 | z);

    read_From(MPU_WHO_AM_I, MPU_GYRO_XOUT_L);
    signed int a = read_Data();
    read_From(MPU_WHO_AM_I, MPU_GYRO_XOUT_H);
    xGyro = (read_Data() << 8 | a);

    read_From(MPU_WHO_AM_I, MPU_GYRO_YOUT_L);
    signed int b = read_Data();
    read_From(MPU_WHO_AM_I, MPU_GYRO_YOUT_H);
    yGyro = (read_Data() << 8 | b);

    read_From(MPU_WHO_AM_I, MPU_GYRO_ZOUT_L);
    signed int c = read_Data();
    read_From(MPU_WHO_AM_I, MPU_GYRO_ZOUT_H);
    zGyro = (read_Data() << 8 | c);

    Serial.print("x: " + String(x) + "\n");
    Serial.print("y: " + String(y) + "\n");
    Serial.print("z: " + String(z) + "\n");

    for (int i = 1000; i <= 4000; i++)
    {
      changeFrequency(i);
    }

    switch (LEDState)
    {
    case LEDSMILEY:
      displaySmile();
      alarmOff();
      break;
    case LEDSAD:
      displayFrown();
      break;
    default:
      break;
    }

    switch (myButtonState)
    {
    case waitPress:
      Serial.println("waitPress");
      delayMs(1000);
      break;
    case debouncePress:
      Serial.println("debouncePress");
      myButtonState = waitRelease;
      break;
    case waitRelease:
      Serial.println("waitRelease");
      break;
    case debounceRelease:
      Serial.println("debounceRelease");
      myButtonState = waitPress;
      break;
    default:
      break;
    }
  }
}

ISR(PCINT0_vect)
{

  if (x > 10 && y > 10 && z > 10)
  {
    alarmOn();
    LEDState = LEDSAD;
  }
  if (myButtonState == waitPress)
  {
    Serial.println("debouncePress");
    myButtonState = debouncePress;
  }
  else if (myButtonState == waitRelease)
  {
    myButtonState = debounceRelease;
    LEDState = LEDSMILEY;
  }
}