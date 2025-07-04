#include "stm32f10x.h"                  // Device header
#include "Delay.h" 


int main (void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //打开时钟
    
    GPIO_InitTypeDef GPIO_InitStructure;                // 定义结构体
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // 开漏输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;         // 配置PA0~PA7所有引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // 输出速度50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);              // 应用配置到GPIOA
    
    //实际操作
    GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET); 
    
    while (1)
    {
        GPIO_Write(GPIOA, ~0x0001); //0000 0000 0000 0001
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0002); //0000 0000 0000 0010
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0004); //0000 0000 0000 0500
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0008); //0000 0000 0000 5000
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0010); //0000 0000 0001 0000
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0020); //0000 0000 0010 0000
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0040); //0000 0000 0500 0000
        Delay_ms(500);
        GPIO_Write(GPIOA, ~0x0080); //0000 0000 5000 0000
        Delay_ms(500);
        
    }
}
