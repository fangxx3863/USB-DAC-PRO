#ifndef __STC_H__
#define __STC_H__

#include <intrins.h>
#include <stdio.h>

#include "STC8H.h"
#include "config.h"

// 用户定义
#define MULTIPLE_EC_COUNT 1     // 是否加倍旋钮点数
#define IS_LVD_MUTE       1     // 是否开启低压中断时静音

// 定义别名
#define USB_MUTE P16
#define VOL_MUTE P33
#define VOL_ZCEN P34
#define VOL_SCLK P17
#define VOL_SDI  P13
#define VOL_CS   P54
#define EC_A     P36
#define EC_B     P37
#define EC_PSW   P35
#define USB_TX   P10
#define DAC_RST  P11
#define FOSC     24000000UL   //ISP下载时需将工作频率设置为24MHz


typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned int ushort;
typedef unsigned long ulong;

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;

#endif
