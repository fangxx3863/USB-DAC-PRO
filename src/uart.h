#ifndef __UART_H__
#define __UART_H__
#include "stc.h"

void UART2_config(u8 brt);   // 选择波特率, 2: 使用Timer2做波特率, 其它值: 无效.
void UART2_EchoToCDC(void);
void PrintString2(u8 *puts);

#endif