#include "encoder.h"
#include "stc32_stc8_usb.h"

typedef enum {
    SW_SCAN = 0,
    SW_DEBOUNCE,
    SW_SHORT_PRESS_DET,
    // SW_LONG_PRESS_DET,
} SW_STA;

BOOL   EC_Flag    = 0;
int    EC_Path    = 0;
u8     EC_Dir     = 0;
BOOL   SW_Flag    = 0;
u8     SW_STATUS  = 0;
u16    SW_Counter = 0;

void ScanECSW() {
    // printf("SW_STATUS: %d", (u16)SW_STATUS);
    // if (!EC_SW) printf("EC_SW Press!\n");
    switch (SW_STATUS) {
    case SW_SCAN:
        // printf("SW_SCAN!");
        if (!EC_PSW) SW_STATUS = SW_DEBOUNCE;
        break;

    case SW_DEBOUNCE:
        SW_Counter++;
        if (SW_Counter >= 65535) SW_Counter = 65535;
        if (SW_Counter > 100 && (!EC_PSW)) SW_STATUS = SW_SHORT_PRESS_DET;
        else SW_STATUS = SW_SCAN;
        break;

    case SW_SHORT_PRESS_DET:
        // SW_Counter++;
        // if (SW_Counter >= 65534) SW_Counter = 65534;
        if (EC_PSW) {
            SW_Flag = 1;
            SW_Counter = 0;
            SW_STATUS = SW_SCAN;
        }
        // else if ((!EC_SW) && SW_Counter > 10000) SW_STATUS = SW_LONG_PRESS_DET;
        break;

    // case SW_LONG_PRESS_DET:
    //     // 处理长按操作
    //     SW_STATUS = SW_SCAN;
    //     SW_Counter = 0;
    //     break;
    
    default:
        SW_STATUS = SW_SCAN;
        break;
    }
}

BOOL GetSWFlag() {
    BOOL tmp = SW_Flag;
    SW_Flag = 0;
    return tmp;
}

int GetECPath() {
    int tmp = EC_Path;
    EC_Path = 0;
    return tmp;
}

BOOL GetECFlag() {
    BOOL tmp = EC_Flag;
    EC_Flag = 0;
    return tmp;
}

u8 GetECDir() {
    return EC_Dir;
}

/********************* INT2中断函数 *************************/
void INT2_int (void) interrupt 10     //进中断时已经清除标志
{
    if (EC_Dir >= 255) EC_Dir = 254;
    if (EC_Dir <= 1) EC_Dir = 1;
    if (EC_B) {
        EC_Path = 1;
        if (EC_Dir < 255) EC_Dir++;
    } else {
        EC_Path = -1;
        if (EC_Dir > 0) EC_Dir--;
    }
    EC_Flag = 1;
    // printf("INT2!\n");
}

/********************* INT3中断函数 *************************/
void INT3_int (void) interrupt 11     //进中断时已经清除标志
{
    #if MULTIPLE_EC_COUNT
    if (EC_Dir >= 255) EC_Dir = 254;
    if (EC_Dir <= 1) EC_Dir = 1;
    if (EC_A) {
        EC_Path = -1;
        if (EC_Dir < 255) EC_Dir--;
    } else {
        EC_Path = 1;
        if (EC_Dir > 0) EC_Dir++;
    }
    EC_Flag = 1;
    // printf("INT3!\n");
    #endif
}