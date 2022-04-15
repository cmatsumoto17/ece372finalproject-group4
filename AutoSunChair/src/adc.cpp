/* ADC 
    1.One function exists to initialize the ADC

    the ADC

*/

#include <avr/io.h>


void initADC(){

//sets the reference voltages for the ADC
//REFS1 = 0, REFS0 = 1 uses Vcc = 5V as the reference voltage
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

//right justify the bits from the ADC by setting ADLAR bit to 0
    ADMUX &= ~(1 << ADLAR);

//specify ADC input channel and mode
//set positive differential input to ADC0 and negative differential input to ADC1 MUX[5:0] = 0b'010000
    ADMUX |= (1 << MUX4);
    ADMUX &= ~( (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0) );
    
    ADCSRB &= ~(1 << MUX5);

//set Auto Trigger Source selection
//use free-running mode ADTS[2:0] = 0b000
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

//enable ADC and enable auto-triggering
    ADCSRA|= (1 << ADEN) | (1 << ADATE);

//set ADC clock frequency, 
//pwm has frequency of 15.625kHz, must sample at twice that rate so desired adc freq >= 31.25kHz
//Use prescaler of 32, 
//ADC clock freq is 16Mhz divided by prescaler = 500kHz
//sampling rate is 1/ (1/500kHz) *(13 clock cycles)) = 38.461kHz
//ADPS[2:0] = 0b101
    ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
    ADCSRA &= ~(1 << ADPS1);

//disable digital input mode for pin ADC0 and ADC1
    DIDR0 |= (1 << ADC0D) | (1 << ADC1D);

//start the first ADC conversion
    ADCSRA |= (1 << ADSC);
}


