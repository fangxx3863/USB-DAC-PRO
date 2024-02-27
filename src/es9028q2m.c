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
    0x80, // Register #7:  General Settings (Default = 0x80)
    0x88, // Register #8:  GPIO Configuration (Default = 0x88)
    0x00, // Register #9:  Empty
    0x02, // Register #10: Master Mode Control (Default = 0x02)
    0x32, // Register #11: Channel Mapping (Default = 0x02)
    0xFF, // Register #12: DPLL/ASRC Settings (Default = 0x5A)
    0x00, // Register #13: THD Compensation (Default = 0x40)
    0x8A, // Register #14: Soft Start Settings (Default = 0x8A)
    0x00, // Register #15: Volume 1 (Default = 0x50)
    0x00, // Register #16: Volume 2 (Default = 0x50)
    // ... (其他寄存器的默认值未在数据手册中提供，因此无法列出)

};

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


}

u8 ES9028_GetChipStatus(void) {
    u8 tmp = 0;
    I2CReadNbyte(0x90, 64, &tmp, 1);
    return tmp;
}