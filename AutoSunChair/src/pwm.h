//Description:  Function prototypes for pwm.cpp
//-------------------------------------------------------

#ifndef PWM_H
#define PWM_H

void initPWM();

/*the changeDutyCycle function takes in an argument which is the
combination of the analog to digital conversion, ADCH and ADCL
registers. The result of the combination of the ADCH and ADCL
registers is a 10-bit number. From this you will edit the OCRnA
registers for the two 10-bit PWM timers used for the lab. Editing
the OCRnA register changes the duty cycle of the PWM signal */
void changeDutyCycle(int adcNum);

void changeDutyCycleAuto(int adcNum);

#endif
