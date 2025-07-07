#include "stm32f10x.h"                  // Device header


void PWM_Init(void)
{
    // RCC开启始终 打开需要用的TIM和GPIO外设
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 配置时基单元：时钟源选择和时基单元
    TIM_InternalClockConfig(TIM2);      // 内部时钟
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;       // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;     // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    // 配置输出比较单元：CRR值、输出比较模式，极性选择，输出使能
    TIM_OCInitTypeDef TIM_InitStructure;
    TIM_OCStructInit(&TIM_InitStructure);       // 给每个结构体参数赋一个初始值
    TIM_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_InitStructure.TIM_Pulse = 50;         // CCR的值为：500-2500
    TIM_OC3Init(TIM2, &TIM_InitStructure);
    
    // 同一定时器不同通道输出的PWM共用一个计时器，频率相同

    // 配置PWM对应GPIO口,复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     // 设置为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 运行控制 启动计数器
    TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare3(uint16_t Compare)
{
    TIM_SetCompare3(TIM2, Compare);
}
