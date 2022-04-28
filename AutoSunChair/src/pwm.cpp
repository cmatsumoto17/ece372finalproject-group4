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


void changeDutyCycle(int adcNum){   // duty cycle  = pulse/period  = OCRnX/ICRn

    // if((-512 < adcNum) && (0 >= adcNum)){ 
    //     OCR3A = OCR3A - 1;
    // }
    // else if ((0 < adcNum) && ( 511 >= adcNum)){ 
    //     OCR3A = OCR3A + 1;
        
    // }
   
    // OCR3A = 2.77777*(adcNum) + 500 ;

    OCR3A = adcNum;

}
void changeDutyCycleAuto(int adcNum) {
  //  float result = (adcNum * .97847)
//   int conversion = (int) result;
//      if((-512 < adcNum) && (0 >= adcNum)){  // toward 1 ms pulse (-90) 5% duty cycle
//             OCR3A = OCR3A  + conversion;
//}
//     else if((0 < adcNum) &&  ((511 >= adcNum)){ // toward 2 ms pulse (90) 10% duty cycle
//              OCR3A = OCR3A + conversion;
// }

    //when shining a light directly above, adc difference fluctuates from -140 to -120
    //if difference is in this range stay put

    
    if(adcNum < -150){ 
        OCR3A -= 1;         //move left if below threshold
    }
    else if(adcNum > -110){
        OCR3A += 1;         //move right if over threshold
    }

    if(OCR3A > 2550){   
        OCR3A = 2550;       //cap OCR3A at 2550 since this is max duty cycle for the servo pwm
    }
    else if (OCR3A < 1000){
        OCR3A = 1000;       //cap lower bound of OCR3A since this in min duty cycle for servo pwm
    }
}