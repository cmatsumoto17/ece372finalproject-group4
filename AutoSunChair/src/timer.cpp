/* timer.cpp
1.Implement a precise millisecond timer using timer 1 for switch
    debouncing.
*/

#include <timer.h>
#include <avr/io.h>

/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
	//set timer 1 into CTC mode by placing 1 into WGM12. all others are zero
    TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B|= (1 << WGM12);
    TCCR1B &= ~(1 << WGM13);
}

/* This delays the program an amount of microseconds specified by unsigned int delay.
*/
void delayUs(unsigned int delay){
    // start the clock with prescaler 8 and OCR1A = 250
    TCCR1B &= ~((1 << CS12)| (1 << CS10));
    TCCR1B |= (1 << CS11);
    
    //OCR1A = 250;
    OCR1AH = 0;
    OCR1AL = 250;

    TIFR1 |= (1 << OCF1A); // set flag down
    TCNT1 = 0; // clear timer

    for (int i = 0; i < delay; i++) {
        while (!(TIFR1 & (1 << OCF1A))); // do nothing while flag is down
        // exit once flag goes up

        TIFR1 |= (1 << OCF1A); // sets flag down
        TCNT1 = 0; // reset timer
    }

    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // turn off clock
}

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){
    // initizlize timer 0 to CTC mode
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);
}

/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
    OCR0A = 250; // sets OCR0A to 250 for 1 ms delay
    TIFR0 |= (1 << OCF0A); // sets flag down
    TCNT0 = 0; // clears timer
    TCCR0B |= (1 << CS01) | (1 << CS00); // sets prescaler to 64 and starts timer
    TCCR0B &= ~(1 << CS02);

    for (int i = 0; i < delay; i++) {
        while (!(TIFR0 & (1 << OCF0A))); // do nothing while flag is down
        // exit once flag goes up

        TIFR0 |= (1 << OCF0A); // sets flag down
        TCNT0 = 0; // reset timer
    }

    TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // turn off clock
}