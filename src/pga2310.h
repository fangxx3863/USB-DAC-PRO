#ifndef __PGA2310_H__
#define __PGA2310_H__
#include "stc.h"

#define MAX_GAIN 254

void SPIWrite(uint8_t b);
void SetVolume(uint8_t left, uint8_t right);


#endif