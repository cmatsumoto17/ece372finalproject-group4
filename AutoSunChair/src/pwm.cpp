//pwm used as a signal input for the servo motor


#include <pwm.h>

#include <avr/io.h>

/* PWM 
    1.Uses a PWMs on timer 3 and timer 4.
        - Physical header pins 5 and 6 should be used. Pin 5 relates
            to timer 3 and pin 6 relates to timer 4.
        - Note:  You will need to demo your PWM signal coming from
            timer 3 using the digital storage oscilloscope in the lab.
    

    2.Has a changeDutyCycle function.
        - the changeDutyCycle function takes in an argument which is the
            combination of the analog to digital conversion, ADCH and ADCL
            registers. The result of the combination of the ADCH and ADCL
            registers is a 10-bit number. From this you will edit the OCRnA
            registers for the two 10-bit PWM timers used for the lab. Editing
            the OCRnA register changes the duty cycle of the PWM signal.
*/

void initPWM(){
    DDRE |= (1 << DDE3); //pin 5 to output
    
    TCCR3A |= (1 << COM3A1);  //set timer 3 to non inverting mode 
    TCCR3A &= ~(1 << COM3A0);

    TCCR3B |=  (1<<WGM33);
    TCCR3B &=  ~(1<<WGM32);
    TCCR3A |=  (1<<WGM31);
    TCCR3A &= ~(1<<WGM30); // PWM, Phase Correct with ICR3 TOP value
    

    ICR3 = 20000;  // pwm frequency = f_clk/((Prescaler)*(1+TOP)) = 50, TOP = 16MHz/(8*50Hz) - 1 = 20000


    //CS2:0 = 0b'011
    TCCR3B &= ~(1<<CS32);  //start clock with prescaler 8 timer 3
    TCCR3B |= (1<<CS31);       
    TCCR3B &= ~(1<<CS30);

    OCR3A = 1500;

   

}

//function changes the duty cycle to move the servo left/right
void moveDirection(int dir){   

    //input dir = 0 corresponds to moving left
    if(dir == 0){    //
        OCR3A += 100;
    }
    //input dir = 1 corresponds to moving right
    else if(dir == 1){
        OCR3A -= 100;
    }
    
    if(OCR3A > 2550){   
        OCR3A = 2550;       //cap OCR3A at 2550 since this is max duty cycle for the servo pwm
    }
    else if (OCR3A < 550){
        OCR3A = 550;       //cap lower bound of OCR3A since this in min duty cycle for servo pwm
    }
}


//function automatically changes the duty cycle of the pwm for a differential input
void changeDutyCycleAuto(int adcNum) {
    //calibration is calculated based on the stable adcDiff reading when a light is shined direclty above
    int calibration = -50;

    //adjust adcNum to be centered around calibrated value
    adcNum += calibration;


    //when shining a light directly above, adc difference fluctuates from -20 to 20 around the stable value
    //if difference is in this range stay put
    if(adcNum < -20 + calibration){ 
        OCR3A -= 100;         //move left if below threshold
    }
    else if(adcNum > 20 + calibration){
        OCR3A += 100;         //move right if over threshold
    }

    if(OCR3A > 2550){   
        OCR3A = 2550;       //cap OCR3A at 2550 since this is max duty cycle for the servo pwm
    }
    else if (OCR3A < 550){
        OCR3A = 550;       //cap lower bound of OCR3A since this in min duty cycle for servo pwm
    }
}