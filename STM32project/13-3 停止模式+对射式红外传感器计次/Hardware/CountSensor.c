#include "stm32f10x.h"                  // Device header
#include "Delay.h"


uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
    // 配置时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    // 打开两个时钟，AFIO和EXTI是默认打开的时钟，NVIC是内核外设自动开启
    
    // 配置GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);    // 配置输入终端为GPIOB，输出为EXTI中断线路14
    
    // 配置EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line14; // 中段线路
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;   // 中断线的新状态
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //指定触发模式，中断和事件
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;     // 指定有效边沿(上升、下降、and)
    EXTI_Init(&EXTI_InitStructure);
    
    // 配置NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置分组方式，整个代码只能分组一次
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;    // 指定通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         // 指定中断通道是使能还是失能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // 配置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // 配置响应优先级
    NVIC_Init(&NVIC_InitStructure);
    
}

uint16_t CountSensor_Get(void)
{
    return CountSensor_Count;
} 

void EXTI15_10_IRQHandler(void)     // 中断函数的名字一般都是从启动函数中复制
{
    if (EXTI_GetITStatus(EXTI_Line14) == SET)   // 检查终端标志位是否为1
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14 == 0))
        {
            CountSensor_Count ++;
        }
        EXTI_ClearITPendingBit(EXTI_Line14);    //请除中断标志位
    }
}
