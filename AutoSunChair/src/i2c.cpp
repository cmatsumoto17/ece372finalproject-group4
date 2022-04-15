/* i2c.cpp
1. InitI2C()
    - Wake up I2C module on mega 2560
    - Set prescaler TWPS to 1
    - Set two wire interface bit rate register TWBR
    - Enable two wire interface

2. StartI2C_Trans(unsigned char SLA)
    - Clear TWINT, initiate start condition, initiate enable
    - Wait for completion
    - Set two wire data register to the SLA + write bit
    - Trigger action: Clear TWINT and initiate enable
    - Wait for completion

3. StopI2C_Trans()
    - Trigger action + stop condition
    - Write(unsigned char data)
    - Set two wire data register equal to incoming data
    - Trigger action
    - Wait for completion

4. Read_from(unsigned char SLA, unsigned Char MEMADDRESS)
    - Start a transmission to the SLA
    - Write to the MEMADDRESS
    - Clear TWINT, initiate start condition, initiate enable
    - Wait for completion
    - Set two wire data register to the SLA + read bit
    - Trigger action + master acknowledge bit
    - Wait for completion
    - Trigger action
    - Wait for completion
    - Trigger action + stop condition

5. unsigned char Read_data()
    - Return TWDR
*/

#include <i2c.h>
#include <avr/io.h>

//wait for the TWINT flag to be set. 
//Flag being set indicates transmission complete
#define wait_for_completion while(!(TWCR & (1 << TWINT)));

void initI2C() {

    PRR0 &= ~(1 << PRTWI);  //wake up I2C module on AT2560
    TWSR |= (1 << TWPS0);   //prescalar power = 1
    TWSR &= ~(1 << TWPS1);  //prescalar power = 1

    //𝑇𝑊𝐵𝑅=((𝐶𝑃𝑈 𝐶𝑙𝑜𝑐𝑘 𝑓𝑟𝑒𝑞𝑢𝑒𝑛𝑐𝑦)/(𝑆𝐶𝐿 𝑓𝑟𝑒𝑞𝑢𝑒𝑛𝑐𝑦)−16)/(2∗〖(4)〗^𝑇𝑊𝑃𝑆 )
    //     =((16MHz) / (400kHz) - 16)/(2*(4)^1)
    //SCL freq =400k, TWPS = 1

    TWBR = 3; // bit rate generator = 400k  (TWBR = 3)

    TWCR |= (1 << TWINT )| (1 << TWEN); // enable two wire interface
}
void StopI2C_Trans(){
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void StartI2C_Trans(unsigned char SLA) {
// this function initiates a start condition and calls slave device with SLA
  
  //send start condition
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // clear TWINT, intiate a start condition and enable
  
  wait_for_completion;

  //loads the slave address into TWDR, write bit set to 0
  TWDR = (SLA << 1); 

  //clear TWINT bit to start the transmission of address
  //enable TWI
  TWCR = (1<<TWINT)|(1<<TWEN);  
  
  wait_for_completion;
}


void write(unsigned char data) {
  // this function loads the data passed into the I2C data register and transmits

  TWDR = data; // load data into TWDR register

  TWCR = (1 << TWINT) | (1 << TWEN);  // trigger action:  clear flag and enable TWI

  wait_for_completion;
}

void Read_from(unsigned char SLA, unsigned char MEMADDRESS) {
  // this function sets up reading from SLA at the SLA MEMADDRESS 

  StartI2C_Trans(SLA);
 
  write(MEMADDRESS);
  
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); // restart to switch to read mode
  wait_for_completion;

  TWDR = (SLA << 1) | 0x01; // 7 bit address for slave plus read bit
  TWCR = (1 << TWINT) | (1 << TWEN)| (1 << TWEA);// trigger with master sending ack
  wait_for_completion;

  TWCR = (1<< TWINT) | (1 << TWEN);  // master can send a nack now
  wait_for_completion;

  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); // Stop condition
// after this function is executed the TWDR register has the data from SLA that Master wants to read
}

unsigned char Read_data() // Returns the last byte  from the data register
{
  return TWDR;
}