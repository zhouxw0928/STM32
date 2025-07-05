#include "stm32f10x.h"                  // Device header


int16_t Encoder_Count;

void Encoder_Init(void)
{
    // 配置时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    // 打开两个时钟，AFIO和EXTI是默认打开的时钟，NVIC是内核外设自动开启
    
    // 配置GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;          // 输入上拉
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);    // 配置输入终端为GPIOB，输出为EXTI中断线路0
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);    // 配置输入终端为GPIOB，输出为EXTI中断线路1
    
    // 配置EXTI, 将GPIOB的引脚0和1与外部中断线路0（EXTI_Line0）和线路1（EXTI_Line1）关联
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1; // 配置中段线路
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;   // 中断线的使能状态
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //指定触发模式：中断或事件
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     // 指定有效边沿(上升、下降、and)
    EXTI_Init(&EXTI_InitStructure);
    
    // 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置分组方式，整个代码只能分组一次.抢占优先级占两位，响应优先级占两位
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;    // 指定通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         // 指定中断通道是使能还是失能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // 配置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // 配置响应优先级
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;    // 指定通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         // 指定中断通道是使能还是失能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // 配置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          // 配置响应优先级
    NVIC_Init(&NVIC_InitStructure);
}

int16_t Encoder_Get(void)
{
    int16_t Temp;
    Temp = Encoder_Count;
    Encoder_Count = 0;
    return Temp;
}    

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) == SET)                // PB0下降沿触发中断EXTI0
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)  // 判断PB1是低电平，此时顺时针旋转
        {
            Encoder_Count++;
        }
        EXTI_ClearITPendingBit(EXTI_Line0);                 //清除中断标志位
    }
}

void EXTI1_IRQHandler()
{
    if (EXTI_GetITStatus(EXTI_Line1) == SET)                // PB1下降沿触发中断EXTI1
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)  // 判断PB0是低电平，此时逆时针旋转
        {
            Encoder_Count--;
        }
        EXTI_ClearITPendingBit(EXTI_Line1);                 //清除中断标志位
    }
}
