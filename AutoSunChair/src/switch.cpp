/* switch.cpp
1. Uses a switch to silence the audio chirping alarm.
*/

#include <switch.h>
#include <avr/io.h>



//MAY NEED TO CHANGE TO EXTERNAL INTERRUPT

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 * header pinA8
 */
void initSwitchPK0(){
    //set direction for input
    DDRK &= ~(1 << DDK0);

    //enable pullup resistor for stable input
    PORTK |= (1 << PORTK0);
 
    //enable pin-change interrupt for PORTK (16-23)
    PCICR |= (1 << PCIE2);

    //turn on pin-change interrupt for PK0
    PCMSK2 |= (1 << PCINT16);
}
