#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h" 
#include "Serial.h" 


uint8_t RxData;

int main (void)
{
    OLED_Init();
    OLED_ShowString(1, 1,"RxData:");

    Serial_Init();

    Serial_SendByte(0x41);
    
    while (1)
    { 
        if (Serial_GetRxFlag() == 1)    // 查询方法通过判断标志位读取
        {
            RxData = Serial_GetRxData();
            Serial_SendByte(RxData);
            OLED_ShowHexNum(1, 8, RxData, 2);
        }
        OLED_ShowString(2, 1, "Running");
        Delay_ms(100);
        OLED_ShowString(2, 1, "       ");
        Delay_ms(100);
        
        __WFI();    // 进入睡眠模式 此时不再刷新Running 再次发送数据会进入串口的中断函数唤醒睡眠
    }
}
