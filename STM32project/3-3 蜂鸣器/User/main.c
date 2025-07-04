#include "stm32f10x.h"                  // Device header
#include "Delay.h" 


int main (void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //打开时钟
    
    GPIO_InitTypeDef GPIO_InitStructure;                // 定义结构体
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // 开漏输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;         // 配置PA0~PA7所有引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 输出速度50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);              // 应用配置到GPIOA
    

    
    while (1)
    {
        
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        Delay_ms(500);
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
        Delay_ms(500);
        
    }
}
