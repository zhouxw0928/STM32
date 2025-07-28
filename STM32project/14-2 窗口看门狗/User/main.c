#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h" 
#include "Key.h" 


int main (void)
{
    OLED_Init();
    Key_Init();
    
    OLED_ShowString(1, 1, "IWDG Strat");
    
    if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) == SET) // 独立看门狗复位
    {
        OLED_ShowString(2, 1, "WWDGRST");
        Delay_ms(500);
        OLED_ShowString(2, 1, "       ");
        Delay_ms(100);
        
        RCC_ClearFlag();
    }
    else
    {
        OLED_ShowString(3, 1, "RST");
        Delay_ms(500);
        OLED_ShowString(3, 1, "       ");
        Delay_ms(100);
    }
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(0X40 | 21);     // 窗口时间30ms
    WWDG_Enable(0X40 | 54);             // 超时时间50ms
    
    while (1)
    { 
        Key_GetNum();
        
        OLED_ShowString(4, 1, "Feed");
        Delay_ms(20);
        OLED_ShowString(4, 1, "    ");
        Delay_ms(20);
        
        WWDG_SetCounter(0X40 | 54);     // 喂狗
    }
}
