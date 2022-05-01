/* switch.cpp
1. Uses a switch to silence the audio chirping alarm.
*/

#include <switch.h>
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB0 and sets it into input mode
 * Header pin 53
 */
void initSwitchPB0() {
    //set direction for input
    DDRB &= ~(1 << DDB0);

    //enable pullup resistor for stable input
    PORTB |= (1 << PORTB0);
 
    //enable pin-change interrupt for PORTB
    PCICR |= (1 << PCIE0);

    //turn on pin-change interrupt for PB0
    PCMSK0 |= (1 << PCINT0);
}

/*
 * Initializes pull-up resistor on PJ0 and sets it into input mode
 * Header pin 15
 */
void initSwitchPJ0(){
    //set direction for input
    DDRJ &= ~(1 << DDJ0);

    //enable pullup resistor for stable input
    PORTJ |= (1 << PORTJ0);

    //enable pin-change interrupt for PORTJ
    PCICR |= (1 << PCIE1);

    //turn on pin-change interrupt for PJ0
    PCMSK1 |= (1 << PCINT9);
}

/*
 * Initializes pull-up resistor on PK7 and sets it into input mode
 * Header pin A15
 */
void initSwitchPK7() {
    //set direction for input
    DDRK &= ~(1 << DDK7);

    //enable pullup resistor for stable input
    PORTK |= (1 << PORTK7);

    //enable pin-change interrupt for PORTK
    PCICR |= (1 << PCIE2);

    //turn on pin-change interrupt for PK7
    PCMSK2 |= (1 << PCINT23);
}
