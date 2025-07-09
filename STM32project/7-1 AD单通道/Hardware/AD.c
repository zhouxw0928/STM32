#include "stm32f10x.h"                  // Device header


void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1,ADC_SampleTime_55Cycles5);  // 序列1的位置写入通道0
    
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_NbrOfChannel = 1;

    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_Cmd(ADC1, ENABLE);
    
    ADC_ResetCalibration(ADC1);     // 标志位 置1开始复位校准
    while (ADC_GetResetCalibrationStatus(ADC1) == SET); // 变为0，复位校准结束
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
//    
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 启动
}

uint16_t AD_GetValue(void)
{
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 启动，连续转换时执行需要启动一次，可以放置在初始化函数里
//    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);     // 转换过程 变为1转换结束
    // 单通道连续转换不需要判断标志位，会不断刷新最新转换结果
    return ADC_GetConversionValue(ADC1);    // 读取
 
}
