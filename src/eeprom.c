#include "eeprom.h"
#define  Tip_Delay (FOSC / 1000000)

//========================================================================
// 函数: void   ISP_Disable(void)
// 描述: 禁止访问ISP/IAP.
// 参数: non.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================
void DisableEEPROM(void)
{
		IAP_CONTR = 0;          //关闭 IAP 功能
		IAP_CMD = 0;            //清除命令寄存器
		IAP_TRIG = 0;           //清除触发寄存器
		IAP_ADDRH = 0x80;       //将地址设置到非 IAP 区域
		IAP_ADDRL = 0;
}

//========================================================================
// 函数: void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number)
// 描述: 从指定EEPROM首地址读出n个字节放指定的缓冲.
// 参数: EE_address:  读出EEPROM的首地址.
//       DataAddress: 读出数据放缓冲的首地址.
//       number:      读出的字节长度.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================
void EEPROM_read_n(u16 EE_address,u8 *DataAddress,u16 number)
{
    EA = 0;     //禁止中断

		IAP_CONTR = 0x80; //使能 IAP
		IAP_TPS = Tip_Delay; //设置擦除等待参数 24MHz
		IAP_CMD = 1;  //设置 IAP 读命令
		do
		{
				IAP_ADDRH = EE_address / 256; //设置 IAP 地址高字节
				IAP_ADDRL = EE_address % 256; //设置 IAP 地址低字节
				IAP_TRIG = 0x5a; //写触发命令(0x5a)
				IAP_TRIG = 0xa5; //写触发命令(0xa5)
				_nop_();
				*DataAddress = IAP_DATA; //读 IAP 数据
        EE_address++;
        DataAddress++;
    }while(--number);
    DisableEEPROM();
    EA = 1;     //重新允许中断
}


/******************** 扇区擦除函数 *****************/
//========================================================================
// 函数: void EEPROM_SectorErase(u16 EE_address)
// 描述: 把指定地址的EEPROM扇区擦除.
// 参数: EE_address:  要擦除的扇区EEPROM的地址.
// 返回: non.
// 版本: V1.0, 2013-5-10
//========================================================================
void EEPROM_SectorErase(u16 EE_address)
{
    EA = 0;     //禁止中断

		IAP_CONTR = 0x80; //使能 IAP
		IAP_TPS = Tip_Delay;     //设置擦除等待参数 24MHz
		IAP_CMD = 3;      //设置 IAP 擦除命令
		IAP_ADDRH = EE_address / 256; //设置 IAP 地址高字节
		IAP_ADDRL = EE_address % 256; //设置 IAP 地址低字节
		IAP_TRIG = 0x5a; //写触发命令(0x5a)
		IAP_TRIG = 0xa5; //写触发命令(0xa5)
		_nop_();
    DisableEEPROM();
    EA = 1;     //重新允许中断
}

//========================================================================
// 函数: void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number)
// 描述: 把缓冲的n个字节写入指定首地址的EEPROM.
// 参数: EE_address:  写入EEPROM的首地址.
//       DataAddress: 写入源数据的缓冲的首地址.
//       number:      写入的字节长度.
// 返回: non.
// 版本: V1.0, 2012-10-22
//========================================================================
void EEPROM_write_n(u16 EE_address,u8 *DataAddress,u16 number)
{
    EA = 0;     //禁止中断

		IAP_CONTR = 0x80; //使能 IAP
		IAP_TPS = Tip_Delay; //设置擦除等待参数 24MHz
		IAP_CMD = 2;  //设置 IAP 写命令
    do
    {
				IAP_ADDRH = EE_address / 256; //设置 IAP 地址高字节
				IAP_ADDRL = EE_address % 256; //设置 IAP 地址低字节
				IAP_DATA = *DataAddress; //写 IAP 数据
				IAP_TRIG = 0x5a; //写触发命令(0x5a)
				IAP_TRIG = 0xa5; //写触发命令(0xa5)
				_nop_();
        EE_address++;
        DataAddress++;
    }while(--number);

    DisableEEPROM();
    EA = 1;     //重新允许中断
}