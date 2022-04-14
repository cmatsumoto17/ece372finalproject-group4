//Description: 

#include "photoresistor.h"
#include <avr/io.h>

void initPR(){
    //set header pins A0 and A1 to be inputs for the photoresistor
    DDRF &= ~(1 << DDF0 | 1 << DDF1);

    //enable pullup resistor for stable input
    PINF |= (1 << PINF0) | (1 << PINF1);

}
