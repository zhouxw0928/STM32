#include "stm32f10x.h"                  // Device header


uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB,uint16_t Size)
{
    MyDMA_Size = Size;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;        // 外设起始地址
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;        // 外设数据宽度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;             // 外设地址是否增
    DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;            // 存储器起始地址
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;            // 存储器数据宽度
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 存储器地址是否自增
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;      // 传输方向
    DMA_InitStructure.DMA_BufferSize = Size;                // 传输计数器大小
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;           // 是否启用自动重装器
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;             // 触发方式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;   // 优先级
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, DISABLE);
}

void DMA_Transfer(void)     // 开启转运
{
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);   // 转运完成标志位置1

    DMA_ClearFlag(DMA1_FLAG_TC1);   // 转运结束 清楚标志位

}
