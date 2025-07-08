#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 配置PWM对应GPIO口,复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     // 设置为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置时基单元：时钟源选择和时基单元
    TIM_InternalClockConfig(TIM3);      // 内部时钟
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;       // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;     // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    // 初始化输入捕获单元
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;    // 配置通道数 这里选通道1
    TIM_ICInitStructure.TIM_ICFilter = 0xF;             // 选择滤波器
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 选择极性 这里为上升沿出发
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   // 设置分频器 这里为一分频
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //直接连接
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);        // 配置触发源
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);     // 选择从模式为RESET
    
    TIM_Cmd(TIM3, ENABLE);      // 启动定时器
}

uint32_t IC_GetFreq(void)
{
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}
