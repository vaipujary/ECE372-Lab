#ifndef TIMER_H
// hellp world
#define TIMER_H
void initPWMTimer3();
void initPWMTimer4();
void changeDutyCycle(double dutyCycle1, double dutyCycle2);
void changeDutyCycle(unsigned int OCRval);
#endif