#include "es9028q2m.h"
#include "i2c.h"
#include "timer.h"

u8 es9028q2m_register[] = {
    0x00, // Register #0:  System Settings (Default = 0x00)
    0x8C, // Register #1:  Input Configuration (Default = 0x8C)
    0x00, // Register #2:  Empty
    0x00, // Register #3:  Empty
    0x00, // Register #4:  Soft Volume Control 1 (Automute Time) (Default = 0x00)
    0x68, // Register #5:  Soft Volume Control 2 (Automute Level) (Default = 0x68)
    0x4A, // Register #6:  Soft Volume Control 3 and De-emphasis (Default = 0x4A)
    0xC0, // Register #7:  General Settings (Default = 0x80)   FIR滤波器类型及IIR滤波器带宽(0x80=FastRolloff,0xA0=SlowRolloff,0xC0=MinimumPhase)
    0x88, // Register #8:  GPIO Configuration (Default = 0x88)
    0x00, // Register #9:  Empty
    0x02, // Register #10: Master Mode Control (Default = 0x02)
    0x32, // Register #11: Channel Mapping (Default = 0x02)
    0xFF, // Register #12: DPLL/ASRC Settings (Default = 0x5A)  DPLL带宽(0x11=最小,0x5A=中等,0xFF=最大)
    0x00, // Register #13: THD Compensation (Default = 0x40)
    0x8A, // Register #14: Soft Start Settings (Default = 0x8A)
    0x00, // Register #15: Volume 1 (Default = 0x50)
    0x00, // Register #16: Volume 2 (Default = 0x50)
    // ... (其他寄存器的默认值未在数据手册中提供，因此无法列出)

};


// 麻了，完全不知道ESS的FIR到底啥格式，量化到24Bits到底啥意思（（（
// int fir[128] = {
//   0,-1,-1,0,2,-2,-7,2,15,-5,-33,6,59,-16,-110,27,180,-60,-294,109,445,-209,-664,360,938,-616,-1299,988,1720,
//   -1551,-2224,2330,2761,-3421,-3332,4860,3850,-6763,-4282,9172,4490,-12220,-4391,15963,3776,-20561,-2481,26119,
//   176,-32904,3487,41225,-9168,-51784,17866,65842,-31723,-86476,55731,122202,-106458,-209418,288441,967649,967649,
//   288441,-209418,-106458,122202,55731,-86476,-31723,65842,17866,-51784,-9168,41225,3487,-32904,176,26119,-2481,
//   -20561,3776,15963,-4391,-12220,4490,9172,-4282,-6763,3850,4860,-3332,-3421,2761,2330,-2224,-1551,1720,988,-1299,
//   -616,938,360,-664,-209,445,109,-294,-60,180,27,-110,-16,59,6,-33,-5,15,2,-7,-2,2,0,-1,-1,0
// };


void ES9028_Init(void) {
    u8 i = 0, tmp = 0;
    DAC_RST = 0;
    delay(10);
    DAC_RST = 1;
    delay(10);
    for (i=0; i<sizeof(es9028q2m_register); i++) {
        I2CWriteNbyte(0x90, i, &es9028q2m_register[i], 1);
    }
    tmp = 0x00;
    I2CWriteNbyte(0x90, 21, &tmp, 1);
    // tmp = 0x00;
    // I2CWriteNbyte(0x90, 15, &tmp, 1);
    // I2CWriteNbyte(0x90, 16, &tmp, 1);

    // // 自定义FIR滤波器_TODO
    // for (i=0; i<sizeof(fir); i++) {
    //     // 位置
    //     I2CWriteNbyte(0x90, 26, &i, 1);
    //     // 数据
    //     tmp = (u8)(fir[i] & 0xFF);
    //     I2CWriteNbyte(0x90, 27, &tmp, 1);
    //     tmp = (u8)((fir[i] >> 8) & 0xFF);
    //     I2CWriteNbyte(0x90, 28, &tmp, 1);
    //     tmp = (u8)((fir[i] >> 16) & 0xFF);
    //     if (fir[i] & 0x8000) tmp|=0x80;
    //     I2CWriteNbyte(0x90, 29, &tmp, 1);
    //     // 写入
    //     tmp = 0x02;
    //     I2CWriteNbyte(0x90, 30, &tmp, 1);
    // }
    // // 完成写入
    // tmp = 0x00;
    // I2CWriteNbyte(0x90, 30, &tmp, 1);


}

u8 ES9028_GetChipStatus(void) {
    u8 tmp = 0;
    I2CReadNbyte(0x90, 64, &tmp, 1);
    return tmp;
}