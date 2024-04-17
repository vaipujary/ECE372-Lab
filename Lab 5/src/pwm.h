#ifndef PWM_H
#define PWM_H

#include <avr/io.h>

void initPWMTimer3();
void changeFrequency();
void alarmOn();
void alarmOff();

#endif