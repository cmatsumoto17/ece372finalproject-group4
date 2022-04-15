//Description: Function prototypes for i2c.cpp

#ifndef I2C_H
#define I2C_H

void initI2C();
void StartI2C_Trans(unsigned char);
void StopI2C_Trans();
void Read_from(unsigned char, unsigned char);
unsigned char Read_data();

#endif