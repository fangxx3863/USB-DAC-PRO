#ifndef __EEPROM_H__
#define __EEPROM_H__
#include "stc.h"
#define  EE_ADDRESS  0x0000  //保存的EEPROM起始地址

void DisableEEPROM(void);
void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number);
void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number);
void EEPROM_SectorErase(u16 EE_address);

#endif