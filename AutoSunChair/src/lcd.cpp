// Author: Cameron Matsumoto
// Net ID: 23517119
// Date: 2/21/2022
// Assignment: Lab 3
//----------------------------------------------------------------------//
#include "lcd.h"
#include "timer.h"
#include <util/delay.h>

/*
 * Initializes all pins related to the LCD to be outputs
 */
void initLCDPins(){

    //PORTA0 - A3 used for the LCD data pins, set to output
    //PORTA0 is LSB
    DDRA |= ((1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3));

    //PORTB4 used for enable pin, set to output
    DDRB |= (1<<DDB4);

    //PORTB6 used for the RS pin, set to output
    DDRB |= (1<< DDB6);
}



/* 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *      a. data is an unsigned char which has 8 bits. Therefore, you
 *         need assign the bottom 4 bits of "data" to the appropriate bits in
 *         PORTA
 *  2. This is a "command" signal, meaning RS should be Low
 *  3. Assert high on enable pin, delay, and asset low on enable pin
 *  4. delay the provided number in MICROseconds.
 */
void fourBitCommandWithDelay(unsigned char data, unsigned int delay){
    //mask the PORTA bits with the new data containing the 4 bit command
    PORTA = data;
    
    //command signal, so RS set to low
    PORTB &= ~(1 << PORTB6);

    //set the enable pin to high
    PORTB |= (1 << PORTB4);

    //delay 1us, min time enable should be on is 230ns
    delayUs(1);

    //set the enable pin to low
    PORTB &= ~(1 << PORTB4);

    //delay for amount in uS passed into function
    delayUs(delay);
}



/* Similar to fourBitCommandWithDelay except that now all eight bits of command are
 * sent.
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 *       a. unlike fourBitCommandWithDelay, you need to send the TOP four bits of
 *          "command" first. These should be assigned to appropriate bits in PORTA
 * 2. This is a command signal, meaning RS should be low
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of command to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay the provided number in MICROseconds.
 */
void eightBitCommandWithDelay(unsigned char command, unsigned int delay){
    //set pins connected to DB[7:4] to the upper 4 bits of the command
    PORTA = (PORTA & 0xF0) | (command >> 4 & 0x0F);

    //command signal, so RS set to low
    PORTB &= ~(1 << PORTB6);

    //set the enable pin to high
    PORTB |= (1 << PORTB4);

    //delay 1us, min time enable should be on is 230ns
    delayUs(1);

    //set the enable pin to low
    PORTB &= ~(1 << PORTB4);
    
    //set pins connected to DB[7:4] to the lower 4 bits of the command
    PORTA = (PORTA & 0xF0) | (command & 0x0F);

    //set the enable pin to high
    PORTB |= (1 << PORTB4);

    //delay 1us, min time enable should be on is 230ns
    delayUs(1);

    //set the enable pin to low
    PORTB &= ~(1 << PORTB4);

    //delay the specified amount of microseconds
    delayUs(delay);
}



/* Similar to eightBitCommandWithDelay except that now RS should be high
 * 1. Assert bits to pins connected to DB[7:4] on the LCD screen
 * 2. This is a "data" signal, meaning RS should be high
 * 3. Assert high on enable pin, delay, and asset low on enable pin
 * 4. Now set the lower four bits of character to appropriate bits in PORTA
 * 5. Assert high on enable pin, delay, and asset low on enable pin
 * 6. delay is always 46 MICROseconds for a character write
 */
void writeCharacter(unsigned char character){
    //set pins connected to DB[7:4] to the upper 4 bits of the command
    //access upper 4 bits by shifting right
    PORTA = (PORTA & 0xF0) | (character >> 4 & 0x0F);

    //set RS to high to write to display
    PORTB |= (1 << PORTB6);

    //set the enable pin to high
    PORTB |= (1 << PORTB4);

    //delay 1us, min time enable should be on is 230ns
    delayUs(1);

    //set the enable pin to low
    PORTB &= ~(1 << PORTB4);
    
    //set pins connected to DB[7:4] to the lower 4 bits of the command
    PORTA = (PORTA & 0xF0) | (character & 0x0F);

    //set the enable pin to high
    PORTB |= (1 << PORTB4);

    //delay 1us, min time enable should be on is 230ns
    delayUs(1);

    //set the enable pin to low
    PORTB &= ~(1 << PORTB4);

    //delay for 46us, always this duration for character write
    delayUs(46);
}




/*
 * Writes a provided string such as "Hello!" to the LCD screen. You should
 * remember that a c string always ends with the '\0' character and
 * that this should just call writeCharacter multiple times.
 */
void writeString(const char *string){
    //move cursor to beginning position for text
    
    //iterate through each character of the string, end of string signified by '\0'
    while(*string != '\0'){

      //write the current character
      writeCharacter(*string);

      //increments the pointer to the next address, which will move to the next character
      string++;
    }
}



/*
 * This moves the LCD cursor to a specific place on the screen.
 * This can be done using the eightBitCommandWithDelay with correct arguments
 */
void moveCursor(unsigned char x, unsigned char y){
	//determine correct DDRAM addres given input x and y
    //addresses are 00 01 02 03 04 .... 0F
    //              40 41 42 43 44 .... 4F
    unsigned char addr = 0x80 | x << 6 | y;

    //run the command with the address to move the cursor
    //moving cursor always delays for 46us
    eightBitCommandWithDelay(addr, 53);
}


/* This is the procedure outline on the LCD datasheet page 4 out of 9.
 * This should be the last function you write as it largely depends on all other
 * functions working.
 */
void initLCDProcedure(){
  // Delay 15 milliseconds
      delayMs(15);

  // Write 0b0011 to DB[7:4] and delay 4100 microseconds
      fourBitCommandWithDelay(0b0011, 4300);

  // Write 0b0011 to DB[7:4] and delay 100 microseconds
      fourBitCommandWithDelay(0b0011, 150);

  // The data sheet does not make this clear, but at this point you are issuing
  // commands in two sets of four bits. You must delay after each command
  // (which is the second set of four bits) an amount specified on page 3 of
  // the data sheet.
  // write 0b0011 to DB[7:4] and 100us delay
      fourBitCommandWithDelay(0b0011, 150);
  // write 0b0010 to DB[7:4] and 100us delay.
      fourBitCommandWithDelay(0b0010, 150);

  // Function set in the command table with 53us delay
  //RS RW DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
  //0  0  0   0   1   DL  N$  RE  *   #
  //DL is 0
  //N is 1 to enable 2 lines
  //RE is 1 to enable external register
  //* and # are 0, * can be anything, # 0 for 16x2 board
        eightBitCommandWithDelay(0b00101000, 70);

  // Display off in the command table with 53us delay
    // 0  0   0   0   0   1   D   C  B
    // D = 1 to toggle display, C = 0 for cursor on/off, cursor blink at position B
        eightBitCommandWithDelay(0b00001000, 70);

  // Clear display in the command table. Remember the delay is longer!!!
        //0 0 0 0 0 0 0 1
        //delay for 1.8 ms to ensure it lasts 82us - 1.64ms
        eightBitCommandWithDelay(0b00000001, 5000);

  // Entry Mode Set in the command table.
        //0 0 0 0 0 1 i/d s
        //i/d = 1/0 = increment/decrement = 1 to increment cursor direction
        //s = 0 always
        eightBitCommandWithDelay(0b00000110, 70);
  // Display ON/OFF Control in the command table. (Yes, this is not specified),
  // in the data sheet, but you have to do it to get this to work. Yay datasheets!)
        //0 0 0 0 1 D C B
        //D = 1 to turn On display
        //C = 1 to turn cursor On
        //B = 1 to turn on blink of character
        eightBitCommandWithDelay(0b00001111, 70);

}



/* Initializes Tri-state for LCD pins and calls initialization procedure.
* This function is made so that it's possible to test initLCDPins separately
* from initLCDProcedure which will likely be necessary.
*/
void initLCD(){
  initLCDPins();
  initLCDProcedure();
}