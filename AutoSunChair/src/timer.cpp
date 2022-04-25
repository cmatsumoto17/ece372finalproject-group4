/* timer.cpp
1.Implement a precise millisecond timer using timer 1 for switch
    debouncing.
*/

#include <timer.h>
#include <avr/io.h>


void initTimer1(){
    TCCR1B &= ~(1<<WGM13);
    TCCR1B |=  (1<<WGM12);
    TCCR1A  &= ~(1<<WGM11);      //initializing timer 1 for CTC mode
    TCCR1A &= ~(1<<WGM10);
}

void delayMs(unsigned int delay){
    unsigned int count  = 0;

    while(delay > count){
        TCNT1 = 0;
        OCR1A = 249;            //set the output compare register to 249
        TIFR1 |= (1 << OCF1A);    // set the output compare flag down by writing a logic 1

        TCCR1B &= ~(1<<CS12); //start clock with prescaler of 64
        TCCR1B |=  (1<<CS11);
        TCCR1B |=  (1<<CS10);

        while ((TIFR1 & (1 << OCF1A)) == 0); //while the output compare flag is down, do nothing

        count++;                         // 1ms reached increment count variable
        TCCR1B &= ~((1 << CS10 ) | (1<< CS11) | (1 << CS12)); // turn off the clock
    }
    return;

    
}