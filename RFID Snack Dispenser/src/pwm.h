#ifndef PWM_H
#define PWM_H

void initPWMTimer3();
void changeDutyCycle(double motorDirection);
void changeFrequency(unsigned int frequencyValue);
void motorCW();
void motorCCW();

#endif
