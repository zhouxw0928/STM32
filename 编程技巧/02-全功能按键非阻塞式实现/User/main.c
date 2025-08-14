#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "Key.h" 
#include "OLED.h" 
#include "Timer.h" 


uint16_t Num1, Num2;

int main (void)
{
    OLED_Init();
    Key_Init();
    Timer_Init();
    

    OLED_ShowString(1, 1, "Num1:");
    OLED_ShowString(2, 1, "Num2:");

    while (1)
    {
    /* 示例1 */
        // if (Key_Check(KEY_HOLD))
        // {
        //     Num1 = 1;
        // }
        // else
        // {
        //     Num1 = 0;
        // }

    /* 示例2 */
        // if (Key_Check(KEY_DOWN))
        // {
        //     Num1++;
        // }
        // else if (Key_Check(KEY_UP))
        // {
        //     Num2++;
        // }

    /* 示例3 */
        // if (Key_Check(KEY_SINGLE))
        // {
        //     Num1++;
        // }
        // else if (Key_Check(KEY_DOUBLE))
        // {
        //     Num1 += 100;
        // }
        // else if (Key_Check(KEY_LONG))
        // {
        //     Num1 = 0;
        // }

/* 示例4 */
        if (Key_Check(KEY_1, KEY_SINGLE) || Key_Check(KEY_1, KEY_REPEAT))
        {
            Num1 ++;
        }
        if (Key_Check(KEY_2, KEY_SINGLE) || Key_Check(KEY_2, KEY_REPEAT))
        {
            Num1 --;
        }
        if (Key_Check(KEY_3, KEY_SINGLE))
        {
            Num1 = 0;
        }
        if (Key_Check(KEY_4, KEY_SINGLE))
        {
            Num1 = 999;
        }

        OLED_ShowNum(1, 6, Num1, 5);
        OLED_ShowNum(2, 6, Num2, 5);
    }
}
 
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        Key_Tick();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
