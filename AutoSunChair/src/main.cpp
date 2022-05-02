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
  MANUAL,
  DB_PRESS_A,
  WAIT_RELEASE_A,
  DB_RELEASE_A,
  DB_PRESS_M,
  WAIT_RELEASE_M,
  DB_RELEASE_M
} state_e;

state_e mode = AUTOMATIC;

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

typedef enum {
  WAIT, UPDATELCD
}statelcd;

volatile statelcd lcdstate = WAIT;
volatile state_r r_state = WAIT_PRESS_R;

volatile boolean rotateLeft = false;
volatile boolean rotateRight = false;

int main(){
  sei();

  initADC();
  initSwitchPB0();
  initSwitchPK7();
  initSwitchPJ0();
  initTimer0();
  initTimer1();
  initTimer3();
  initI2C();
  initLCD();
 // initLCDPins();
// initLCDProcedure();
  initPWM();

  Serial.begin(9600);

  StartI2C_Trans(0b01001000);//call the deveice by address
  
  

  int val0, val1, diff;
  unsigned char temp; // temperature variable
  float realtemp=0; // temperature float variable
  float tempfar = 0;
  char tempval[8];
  char tempfaren[8];

moveCursor(0,0);
writeString("Temperature in F");
moveCursor(1,0);

  

  while(1){
  
   
   
    
   



  //state machine for controlling automatic or manual mode
    switch(mode){
      case(AUTOMATIC):
        val0 = readPin0();
        val1 = readPin1();

        diff = val0 - val1;

        // Serial.print("Pin 0 ");
        // Serial.print(val0);
        // Serial.print("\n");

        // Serial.print("Pin 1 ");
        // Serial.print(val1);
        // Serial.print("\n");

        Serial.print("Diff ");
        Serial.print(diff);
        Serial.print("\n");

        Serial.flush();

        changeDutyCycleAuto(diff);

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
      case(DB_PRESS_A):
          delayMs(1); // if debouncing, delay then wait for release
          mode = WAIT_RELEASE_A;
      break;
    
      case(WAIT_RELEASE_A):
        //waiting for release do nothing
      break;

      case(DB_RELEASE_A):
         delayMs(1); // if debouncing, delay then set to manual mode
         mode = MANUAL;
      break;

      case(DB_PRESS_M):
          delayMs(1); // if debouncing, delay then wait for release
          mode = WAIT_RELEASE_M;
      break;
    
      case(WAIT_RELEASE_M):
        //waiting for release do nothing
      break;

      case(DB_RELEASE_M):
          delayMs(1); // if debouncing, delay then set to automatic mode
          mode = AUTOMATIC;
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
  switch(lcdstate){
    case WAIT:
      break;

      case UPDATELCD:
         Read_from(0b01001000,0b00000000);
        temp = Read_data();
        Serial.print(realtemp);
    

          if(temp & 0x01){
          temp = temp & 0xFE;
      
          realtemp = (float)temp + 0.5;
    }
    else{
      realtemp = temp;
    }
    if(temp & 0x80){
      realtemp = temp*(-1);

    }
    dtostrf(realtemp,6,2,tempval);
    moveCursor(1,0);
    tempfar = (((9/5)*realtemp)+32);
    dtostrf(tempfar,6,2,tempfaren);


    writeString(tempfaren);
    moveCursor(1,0);
    
    Serial.print("\n");
    lcdstate = WAIT;
  }




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

ISR(PCINT1_vect){
  if(mode == AUTOMATIC){ // if interrupt triggered while automatic mode
    mode = DB_PRESS_A; // debounce
  }
  else if(mode == WAIT_RELEASE_A){ // if interrupt triggered while waiting for release
    mode = DB_RELEASE_A; // debounce and set to MANUAL mode
  }
  else if (mode == MANUAL){ // if interrupt triggered while manual mode
    mode = DB_PRESS_M; // debounce
  }
  else if (mode == WAIT_RELEASE_M){ // if interrupt triggered while waiting for release
    mode = DB_RELEASE_M; // debounce and set to AUTOMATIC mode
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

ISR(TIMER3_COMPA_vect){
if(lcdstate == WAIT){
  lcdstate = UPDATELCD;

}


}
