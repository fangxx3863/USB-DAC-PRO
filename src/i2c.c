#include "i2c.h"

// I2C配置寄存器
#define I2CCFG          (*(unsigned char volatile xdata *)0xfe80)
#define I2CMSCR         (*(unsigned char volatile xdata *)0xfe81)
#define I2CMSST         (*(unsigned char volatile xdata *)0xfe82)
#define I2CSLCR         (*(unsigned char volatile xdata *)0xfe83)
#define I2CSLST         (*(unsigned char volatile xdata *)0xfe84)
#define I2CSLADR        (*(unsigned char volatile xdata *)0xfe85)
#define I2CTXD          (*(unsigned char volatile xdata *)0xfe86)
#define I2CRXD          (*(unsigned char volatile xdata *)0xfe87)
#define I2CMSAUX        (*(unsigned char volatile xdata *)0xfe88)
#define DIS_DOT     0x20
#define DIS_BLACK   0x10
#define DIS_        0x11
// #define SLAW    0xA0
// #define SLAR    0xA1

static void Wait()
{
    while (!(I2CMSST & 0x40));
    I2CMSST &= ~0x40;
}

static void Start()
{
    I2CMSCR = 0x01;                         //发送START命令
    Wait();
}

static void SendData(char dat)
{
    I2CTXD = dat;                           //写数据到数据缓冲区
    I2CMSCR = 0x02;                         //发送SEND命令
    Wait();
}

static void RecvACK()
{
    I2CMSCR = 0x03;                         //发送读ACK命令
    Wait();
}

static char RecvData()
{
    I2CMSCR = 0x04;                         //发送RECV命令
    Wait();
    return I2CRXD;
}

static void SendACK()
{
    I2CMSST = 0x00;                         //设置ACK信号
    I2CMSCR = 0x05;                         //发送ACK命令
    Wait();
}

static void SendNAK()
{
    I2CMSST = 0x01;                         //设置NAK信号
    I2CMSCR = 0x05;                         //发送ACK命令
    Wait();
}

static void Stop()
{
    I2CMSCR = 0x06;                         //发送STOP命令
    Wait();
}

void I2CWriteNbyte(u8 addr, u8 reg, u8 *p, u8 number)  /*  WordAddress,First Data Address,Byte lenth   */
{
    Start();                                //发送起始命令
    SendData(addr);                         //发送设备地址+写命令
    RecvACK();
    SendData(reg);                          //发送存储地址
    RecvACK();
    do
    {
        SendData(*p++);
        RecvACK();
    }
    while(--number);
    Stop();                                 //发送停止命令
}

void I2CReadNbyte(u8 addr, u8 reg, u8 *p, u8 number)   /*  WordAddress,First Data Address,Byte lenth   */
{
    Start();                                //发送起始命令
    SendData(addr);                         //发送设备地址+写命令
    RecvACK();
    SendData(reg);                          //发送存储地址
    RecvACK();
    Start();                                //发送起始命令
    SendData(addr|1);                       //发送设备地址+读命令
    RecvACK();
    do
    {
        *p = RecvData();
        p++;
        if(number != 1) SendACK();          //send ACK
    }
    while(--number);
    SendNAK();                              //send no ACK	
    Stop();                                 //发送停止命令
}
/****************************/

