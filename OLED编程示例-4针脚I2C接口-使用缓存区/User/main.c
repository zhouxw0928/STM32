#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h" 
#include <math.h>


extern uint8_t OLED_DisplayBuffer[8][128];

uint8_t Img[]= {
    0xFF,0x01,0xE1,0x11,0x49,0x25,0x45,0x05,0x45,0x25,0x49,0x11,0xE1,0x01,0xFF,
    0x7F,0x40,0x43,0x44,0x48,0x51,0x52,0x52,0x52,0x51,0x48,0x44,0x43,0x40,0x7F,/*"未命名文件",0*/
};

int main (void)
{
    OLED_Init();

    OLED_Clear();
    
    for (uint8_t i = 0; i < 128; i++)
    {
        OLED_DrawPoint(i, sin(i / 123.0 * 2 * 3.14) * 32 + 32);
    }
    
    if (OLED_GetPoint(0, 33))
    {
        OLED_ShowString(0, 0, "YES", 6);
    }
    else{
        OLED_ShowString(0, 0, "NO ", 6);
    }
    

    OLED_Update();
    
//    OLED_ShowChinese(20, 3, "你好世界啊");
    
    while (1)
    { 
        
    }
}
