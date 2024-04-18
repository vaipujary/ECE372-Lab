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

// LED display states
typedef enum
{
  LEDSMILEY,
  LEDSAD
} LEDFACES;

volatile int x = 0;
volatile int y = 0;
volatile int z = 0;

volatile LEDStates ledState = smileQuiet;
volatile buttonState myButtonState = waitPress;
volatile LEDFACES LEDState = LEDSMILEY;
int chirpOn = 0; // chirp = 0 no chirp, 1 chirping

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
    // Read x position
    read_From(SLA, XOUT_HIGH);
    x = read_Data();
    read_From(SLA, XOUT_LOW);
    x = (read_Data() << 8) | x;

    // Read y position
    read_From(SLA, YOUT_LOW);
    y = read_Data();
    read_From(SLA, YOUT_HIGH);
    y = (read_Data() << 8) | y;

    // Read z position
    read_From(SLA, ZOUT_LOW);
    z = read_Data();
    read_From(SLA, ZOUT_HIGH);
    z = (read_Data() << 8) | z;

    Serial.print("x: " + String(x) + "\n");
    Serial.print("y: " + String(y) + "\n");
    Serial.print("z: " + String(z) + "\n");

    // LED State Machine
    switch (LEDState)
    {
    case LEDSMILEY:
      displaySmile();
      break;
    case LEDSAD:
      displayFrown();
      break;
    default:
      break;
    }

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

      if (ledState == frownLoud)
      {
        alarmOff();
        Serial.println("frownQuiet");
        ledState = frownQuiet;
      }
      else if (ledState == smileLoud)
      {
        Serial.println("smileQuiet");
        ledState = smileQuiet;
      }

      myButtonState = waitPress;
      break;

    default:
      break;
    }
  }
  // LED State Machine
  switch (ledState)
  {
  case smileQuiet:
    Serial.println("smileQuiet");

    // Check thresholds of accelerometer: if above threshold, display frown
    if ((y < 0) || (y > 7000) || (z <= 12500))
    {
      LEDState = LEDSAD;
      ledState = frownLoud;
    }
    // Else, display smiley face
    else
    {
      LEDState = LEDSMILEY;
    }
    break;

  case smileLoud:
    Serial.println("smileLoud");

    // Check thresholds of accelerometer: if above threshold, display frown
    if ((y < 0) || (y > 7000) || (z <= 12500))
    {
      LEDState = LEDSAD;
      ledState = frownLoud;
    }
    // Else, display smiley face
    else
    {
      LEDState = LEDSMILEY;
    }

    break;

  case frownQuiet:
    Serial.println("frownQuiet");

    // Check thresholds of accelerometer: if above threshold, display frown
    if ((y < 0) || (y > 7000) || (z <= 12500))
    {
      LEDState = LEDSMILEY;
      ledState = smileQuiet;
    }
    // Else, display smiley face
    else
    {
      LEDState = LEDSAD;
    }
    break;

  case frownLoud:
    Serial.println("frownLoud");

    // Check thresholds of accelerometer: if above threshold, display frown
    if ((y < 0) || (y > 7000) || (z <= 12500))
    {
      LEDState = LEDSMILEY;
      ledState = smileLoud;
    }
    // Else, display smiley face
    else
    {
      LEDState = LEDSAD;
    }

    break;
  default:
    break;
  }

  // Stop I2C transmission
  stopI2C_Trans();
}

// Interrupt Service Routine
ISR(INT2_vect)
{
  if (myButtonState == waitPress)
  {
    Serial.println("waitpress --> debounce press");
    myButtonState = debouncePress;
  }
  else if (myButtonState == waitRelease)
  {
    Serial.println("waitRelease --> debounce release");
    myButtonState = debounceRelease;
  }
}