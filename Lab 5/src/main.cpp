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
#include <avr/interrupt.h>
#include "spi.h"
#include "pwm.h"
#include "timer.h"
#include "switch.h"
#include "i2c.h"

// 59 and 60 for x
// 61 and 62 for y
// 63 and 64 for z
#define XOUT_HIGH 0x3B
#define XOUT_LOW 0x3C
#define YOUT_HIGH 0x3D
#define YOUT_LOW 0x3E
#define ZOUT_HIGH 0x3F
#define ZOUT_LOW 0x40

#define SLA 0x68

#define PWR_MANAGEMENT_REG 0x6B

// Button states
typedef enum
{
  smileQuiet,
  smileLoud,
  frownQuiet,
  frownLoud
} LEDStates;

typedef enum
{
  waitPress,
  debouncePress,
  waitRelease,
  debounceRelease
} buttonState;

volatile int x = 0;
volatile int y = 0;
volatile int z = 0;

volatile LEDStates ledState = smileQuiet;
volatile buttonState myButtonState = waitPress;

int main()
{
  Serial.begin(9600);

  // Initializations
  initI2C();

  // Global interrupt
  sei();
  initPWMTimer3();
  initSPI();
  initTimer1();
  initSwitchPD2();

  startI2C_Trans(SLA);
  // Power management
  write(PWR_MANAGEMENT_REG);
  // Wake up from sleep mode
  write(0);

  // SPI LED Matrix
  // LED Matrix brightness control
  write_execute(0x0A, 0x03);
  // Scan all rows and columns
  write_execute(0x0B, 0x07);
  // Set shutdown register to normal operation
  write_execute(0x0C, 0x01);
  // Set test register to normal operation
  write_execute(0x0F, 0x00);

  while (1)
  {
    for (int i = 1000; i < 4000; i++)
    {
      changeFrequency(i);
    }

    // Read x position
    read_From(SLA, XOUT_HIGH);
    x = read_Data();
    read_From(SLA, XOUT_LOW);
    x = (x << 8) | read_Data();

    // Read y position
    read_From(SLA, YOUT_HIGH);
    y = read_Data();
    read_From(SLA, YOUT_LOW);
    y = (y << 8) | read_Data();

    // Read z position
    read_From(SLA, ZOUT_HIGH);
    z = read_Data();
    read_From(SLA, ZOUT_LOW);
    z = (z << 8) | read_Data();

    Serial.print("x: " + String(x) + "\n");
    Serial.print("y: " + String(y) + "\n");
    Serial.print("z: " + String(z) + "\n");

    // threshold check
    if ((x >= 8000) || (x <= -8000) || (z <= 13000))
    {
      displayFrown();
    }
    else
    {
      displaySmile();
    }

    // Button state machine logic
    switch (myButtonState)
    {
      ///////////////////////////////Press States/////////////////////////////////////////
    case waitPress: // the "natural" state
      // Do nothing, wait for button to be pressed
      Serial.println(1);
      if (!(PIND & (1 << PD2)))
      {
        alarmOff();
      }
      break;

    case debouncePress: // Debounce Press state, wait for swithc debounce state to end
      Serial.println("debouncePress");
      Serial.flush();
      delayMs(1);
      alarmOff();
      myButtonState = waitRelease;
      break;

    ///////////////////////////////Release States///////////////////////////////
    case waitRelease: // waits for button to be released after pressed
      Serial.println("waitRelease");
      Serial.flush();
      delayMs(1);
      alarmOff();
      if (PIND & (1 << PD2))
      {
        myButtonState = debounceRelease;
      }
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

    // LED State Machine
    switch (ledState)
    {
    case smileQuiet:
      Serial.println("smileQuiet");
      alarmOff();
      // Check thresholds of accelerometer: if above threshold, display frown
      if ((x >= 8000) || (x <= -8000) || (z <= 13000))
      {
        ledState = frownLoud;
      }
      // Else, display smiley face
      else
      {
      }
      break;

    case smileLoud:
      Serial.println("smileLoud");

      // Check thresholds of accelerometer: if above threshold, display frown
      if ((x >= 8000) || (x <= -8000) || (z <= 13000))
      {
        alarmOn();
        ledState = frownLoud;
      }
      // Else, display smiley face
      else
      {
      }

      break;

    case frownQuiet:
      Serial.println("frownQuiet");
      alarmOff();
      // Check thresholds of accelerometer: if above threshold, display frown
      if ((x >= 8000) || (x <= -8000) || (z <= 13000))
      {
        ledState = smileQuiet;
      }
      // Else, display smiley face
      else
      {
      }
      break;

    case frownLoud:
      Serial.println("frownLoud");
      // Check thresholds of accelerometer: if above threshold, display frown
      if ((x >= 8000) || (x <= -8000) || (z <= 13000))
      {
        alarmOn();
        ledState = smileLoud;
      }
      // Else, display smiley face
      else
      {
      }

      break;
    default:
      break;
    }
  }
  // Stop I2C transmission
  stopI2C_Trans();
}

// Interrupt Service Routine
ISR(PCINT2_vect)
{
  if (myButtonState == waitPress)
  {
    myButtonState = debouncePress;
  }
  else if (myButtonState == waitRelease)
  {
    myButtonState = debounceRelease;
  }
}