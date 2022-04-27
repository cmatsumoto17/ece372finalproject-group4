#include <avr/io.h>
#include <avr/interrupt.h>

#include "i2c.h"
#include "lcd.h"
#include "servo.h"
#include "switch.h"
#include "timer.h"
#include "adc.h"
#include "pwm.h"



#include "Arduino.h"

typedef enum {
  AUTOMATIC,
  MANUAL
} state_e;

state_e mode = MANUAL;

typedef enum {
  WAIT_PRESS_L,
  DB_PRESS_L,
  WAIT_RELEASE_L,
  DB_RELEASE_L
} state_l;

volatile state_l l_state = WAIT_PRESS_L;

typedef enum {
  WAIT_PRESS_R,
  DB_PRESS_R,
  WAIT_RELEASE_R,
  DB_RELEASE_R
} state_r;

volatile state_r r_state = WAIT_PRESS_R;

volatile boolean rotateLeft = false;
volatile boolean rotateRight = false;

int main(){
  sei();

  initADC();
  initSwitchPB0();
  initSwitchPK7();
  initTimer1();
  //initLCD();
  //initLCDPins();
  //initLCDProcedure();
  initPWM();

  Serial.begin(9600);
  

  unsigned int pResistorDiff; //ADC0 - ADC1
  

  while(1){
  //state machine for controlling automatic or manual mode
    switch(mode){
      case(AUTOMATIC):
        pResistorDiff = ADCL;
        pResistorDiff  += ((unsigned int) ADCH) << 8;

      break;

      case(MANUAL):
        if (rotateLeft) {
          // rotate servo left
          servoLeft();
          delayMs(10);
          servoOff();
          Serial.println("LEFT");
        }
        else if (rotateRight) {
          // rotate servo right
          servoRight();
          delayMs(10);
          servoOff();
          Serial.println("RIGHT");
        }
      break;
    }

    // state machine for left button debouncing
    switch(l_state) {
      case WAIT_PRESS_L:
        break; // if waiting for press, do nothing
      case DB_PRESS_L:
        delayMs(1); // if debouncing, delay then wait for release
        l_state = WAIT_RELEASE_L;
        break;
      case WAIT_RELEASE_L:
        break; // if waiting for release, do nothing
      case DB_RELEASE_L:
        delayMs(1); // if debouncing, delay then wait for press
        l_state = WAIT_PRESS_L;
        break;
    }

    // state machine for right button debouncing
    switch(r_state) {
      case WAIT_PRESS_R:
        break; // if waiting for press, do nothing
      case DB_PRESS_R:
        delayMs(1); // if debouncing, delay then wait for release
        r_state = WAIT_RELEASE_R;
        break;
      case WAIT_RELEASE_R:
        break; // if waiting for release, do nothing
      case DB_RELEASE_R:
        delayMs(1); // if debouncing, delay then wait for press
        r_state = WAIT_PRESS_R;
        break;
    }
  
    // print temperature on LCD
  }
}

ISR(PCINT0_vect) {
  if (l_state == WAIT_PRESS_L) { // if interrupt triggered while waiting for press
    rotateLeft = true; // start rotating

    l_state = DB_PRESS_L; // debounce
  }
  else if (l_state == WAIT_RELEASE_L) { // if interrupt triggered while waiting for release
    rotateLeft = false; // stop rotating
    
    l_state = DB_RELEASE_L; // debounce
  }
}

ISR(PCINT2_vect) {
  if (r_state == WAIT_PRESS_R) { // if interrupt triggered while waiting for press
    rotateRight = true; // start rotating

    r_state = DB_PRESS_R; // debounce
  }
  else if (r_state == WAIT_RELEASE_R) { // if interrupt triggered while waiting for release
    rotateRight = false; // stop rotating
    
    r_state = DB_RELEASE_R; // debounce
  }
}