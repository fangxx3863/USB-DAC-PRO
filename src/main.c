/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序             */
/*---------------------------------------------------------------------*/

/*************  功能说明    **************

基于STC8H8K64U实现的数控模拟音量

下载时, 设置IRC工作频率为 24MHz.

******************************************/

// 导入头文件
#include "stc.h"
#include "stc32_stc8_usb.h"
#include "timer.h"
#include "encoder.h"
#include "pga2310.h"
#include "uart.h"
#include "es9028q2m.h"
#include "eeprom.h"

char *USER_DEVICEDESC = NULL;
char *USER_PRODUCTDESC = NULL;
char *USER_STCISPCMD = "@STCISP#";     //不停电自动ISP下载命令

// 定义中断位
#define EX2         0x10
#define EX3         0x20
#define EX4         0x40
#define DIS_DOT     0x20
#define DIS_BLACK   0x10
#define DIS_        0x11

// 定义全局变量
BOOL _mute_sta;

typedef enum {
    EEPR_SAME = 0,
    EEPR_CHANGE,
    EEPR_SAVE,
} EEPR_STA;
u8  EEPR_STATUS;
u16 EEPR_Counter;


void sys_init();
void main() {
    // 系统初始化
    UART2_config(2);    // 选择波特率, 2: 使用Timer2做波特率, 其它值: 无效.
    sys_init();
    timer_init();
    ES9028_Init();

    // 音量IC调整
    delay(20);      // 等DAC软启动完成
    VOL_ZCEN = 1;   // 打开音量控制过零检测
    VOL_MUTE = 1;   // 关闭静音

    // 读上次断电前数据
    EEPROM_read_n(0x00, &EC_Dir, 1);
    if (GetECDir() == 255) {
        EEPROM_SectorErase(0x00);
        EC_Dir = 128;
        EEPROM_write_n(0x00, &EC_Dir, 1);
    }
    SetVolume(GetECDir(), GetECDir());


    // 主循环
    while (1) {
        if (GetECFlag() && EC_PSW) {
            SetVolume(GetECDir(), GetECDir());
            _mute_sta = 0;
            EEPR_Counter = 0;
            EEPR_STATUS = EEPR_CHANGE;
            // printf("TEST-DEC: %d\n", (u16)66);
            // printf("EC-DIR: %d\n", (u16)GetECDir());
        }
        ScanECSW();
        if (GetSWFlag()) {
            _mute_sta = !_mute_sta;
            if (_mute_sta) SetVolume(0, 0);
            else SetVolume(GetECDir(), GetECDir());
            // printf("SW SHORT PRESS DET!\n");
        }

        // UART2_EchoToCDC();

        // if (!EC_PSW) {
        //     printf("EC_SW Press!\n");
        //     printf("ES9028Q2M #64: %d\n", (u16)ES9028_GetChipStatus());
        // }

        switch (EEPR_STATUS) {
        case EEPR_SAME:
            // 相同值啥也不干
            break;

        case EEPR_CHANGE:
            EEPR_Counter++;
            if (EEPR_Counter >= 65534) EEPR_Counter = 65534;
            if (EEPR_Counter > 5000) EEPR_STATUS = EEPR_SAVE;
            break;

        case EEPR_SAVE:
            EEPROM_SectorErase(0x00);
            EEPROM_write_n(0x00, &EC_Dir, 1);
            EEPR_Counter = 0;
            EEPR_STATUS = EEPR_SAME;
            break;
        
        default:
            break;
        }

    }
}

void sys_init() {
    P_SW2 |= 0x80;  // 扩展寄存器(XFR)访问使能

    P0M0 = 0x00; P0M1 = 0x00; 
    P1M0 = 0x8A; P1M1 = 0x40; 
    P2M0 = 0x00; P2M1 = 0x00; 
    P3M0 = 0x18; P3M1 = 0xE3; 
    P4M0 = 0x00; P4M1 = 0x00; 
    P5M0 = 0x10; P5M1 = 0x00; 
    P6M0 = 0x00; P6M1 = 0x00; 
    P7M0 = 0x00; P7M1 = 0x00; 

    P1PU = 0x30;    // I2C上拉电阻
    DAC_RST = 0;    // DAC_RST下拉

    P3M0 &= ~0x03;  // 设置USB口高阻输入
    P3M1 |= 0x03;

    P3M1 &= 0<<5;   // 设置P35准双向
    P3M0 &= 1<<5;
    
    IRC48MCR = 0x80;    // 设置内部震荡控制器
    while (!(IRC48MCR & 0x01));
    
    USBCLK = 0x00;      // USB时钟设置
    USBCON = 0x90;      // USB控制设置

    IE1  = 0;   // 外中断1标志位
    IE0  = 0;   // 外中断0标志位
    EX1 = 1;    // 使能外中断1
    EX0 = 1;    // 使能外中断0

    IT0 = 1;        //INT0 下降沿中断
    IT1 = 1;        //INT1 下降沿中断
    INTCLKO = EX2;  //使能 INT2 下降沿中断
    INTCLKO |= EX3; //使能 INT3 下降沿中断

    usb_init();
    IE2|= 0x80;                  //使能USB中断

    P_SW2 |= 0x00;              //I2C选择P1.4,P1.5
    P_SW2 &= ~0x01;				//UART2/USART2: RxD2(P1.0), TxD2(P1.1)
    I2CCFG = 0xe0;              //使能I2C主机模式
    I2CMSST = 0x00;

    EA = 1;     //允许总中断
    // while(DeviceState != DEVSTATE_CONFIGURED);              //等待USB完成配置
}

#if IS_LVD_MUTE
/********************** 低压中断函数 ************************/
void LVD_Routine(void) interrupt 6
{
    VOL_MUTE = 0;     // 开启静音
    SetVolume(0, 0);  // 调低音量
}
#endif