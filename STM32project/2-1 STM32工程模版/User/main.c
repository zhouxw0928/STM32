#include "stm32f10x.h"                  // Device header


int main(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);       // 打开时钟
    GPIO_InitTypeDef GPIO_InitStructure;                        //定义结构体变量；配置 GPIO 引脚的工作模式和其他参数
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;            // 引脚将配置为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                  // 指定要配置的 GPIO 引脚是第13号引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           // 输出速度为 50 MHz,表示该引脚的切换速度
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);       //将 GPIOC 的第 13 引脚设置为高电平
//    GPIO_ResetBits(GPIOC, GPIO_Pin_13);     //将13号口设置成低电平
    while(1)
    {
        ;
    }
}
