#include <avr/io.h>
#include <avr/interrupt.h>

#include "i2c.h"
#include "lcd.h"
#include "photoresistor.h"    //may not need photoresitor files
#include "servo.h"
#include "switch.h"
#include "time.h"
#include "adc.h"


enum modes {automatic, manual};

int mode = automatic;

int main(){
  sei();

  initADC();
  initSwitchPK0();
  initLCD();
  initLCDPins();
  initLCDProcedure();
  

  unsigned int pResistorDiff; //ADC0 - ADC1
  

  while(1){
    //state machine for controlling automatic or manual mode
    switch(mode){
      case(automatic):
        pResistorDiff = ADCL;
        pResistorDiff  += ((unsigned int) ADCH) << 8;

      break;

      case(manual):

      break;
    }
    
  }

  

}

ISR(PCINT16_vect){
    
}