//Description: Function prototypes for adc.cpp
//------------------------------------------------------

#ifndef ADC_H
#define ADC_H

//sets up the mode and initialize registers for ADC
void initADC();

//reads ADC conversion from pin A1
int readPin1();

//reads ADC conversion from pin A0
int readPin0();

#endif