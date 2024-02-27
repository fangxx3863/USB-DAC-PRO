#ifndef __ENCODER_H__
#define __ENCODER_H__
#include "stc.h"

void ScanECSW();
int  GetECPath();
BOOL GetECFlag();
u8   GetECDir();
BOOL GetSWFlag();

extern u8 EC_Dir;


#endif