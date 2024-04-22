#ifndef PWM_H
#define PWM_H

void initPWMTimer3();
void changeDutyCycle(double direction);
void changeFrequency(unsigned int i);
void motorCW();
void motorCCW();

#endif
