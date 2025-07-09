#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h" 
#include "Timer.h"
#include "Encoder.h"


int16_t Speed;

int main (void)
{
    OLED_Init();
    Timer_Init();
    Encoder_Init();

    OLED_ShowString(1, 1, "Speed:");

    while (1)
    {
        OLED_ShowSignedNum(1, 7, Speed , 5);    // 读取速度值
    }
}

void TIM2_IRQHandler(void)      // 利用定时器中断实现定时读取Speed，闸门时间=定时器中断时间
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        Speed = Encoder_Get();      // 获取速度值
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
