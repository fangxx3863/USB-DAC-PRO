#ifndef __I2C_H__
#define __I2C_H__
#include "stc.h"

void I2CWriteNbyte(u8 addr, u8 reg, u8 *p, u8 number);
void I2CReadNbyte(u8 addr, u8 reg, u8 *p, u8 number);


#endif