/* Servo SG90 requires a pwm signal, +5V VCC, and ground connection

    PWM signal:
        50Hz (20ms) period, ~5V amplitude, 1-2 ms Duty Cycle (10%-20%)


        When the duty cycle is 1ms, servo at 0 degrees
                               1.5,         90
                                2           180
*/
#include <avr/io.h>
#include "pwm.h"


void servoLeft() {
    moveDirection(0); // 5% duty cycle * TOP (20000)
}

void servoRight() {
    moveDirection(1); // 10% duty cycle * TOP (20000)
}

void servoOff() {
    moveDirection(3); // stop moving
}