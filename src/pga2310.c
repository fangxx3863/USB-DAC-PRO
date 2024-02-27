#include "pga2310.h"

u8 _pv_left, _pv_right, _v_left, _v_right;

void SPIWrite(uint8_t b) {
    u8 i = 0;
    for (i = 0; i < 8; i++) {
        VOL_SCLK = 0;
        if (0x80 & b) {
            VOL_SDI = 1;
        }else {
            VOL_SDI = 0;
        }
        VOL_SCLK = 1;
        b <<= 1;
    }
}

// 由于为了布线方便,调换了原有芯片的音量线序
void SetVolume(uint8_t left, uint8_t right) {
    if ((left > MAX_GAIN) || (right > MAX_GAIN)) {
        return; /* don't allow gains above MAX_GAIN */
    }

    VOL_CS = 0;
    SPIWrite(left);
    SPIWrite(left);
    SPIWrite(right);
    SPIWrite(right);
    VOL_CS = 1;

    _pv_left = _v_left; _pv_right = _v_right;
    _v_left = left; _v_right = right;
}