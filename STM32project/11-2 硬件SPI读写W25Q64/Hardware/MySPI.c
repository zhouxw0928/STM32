#include "stm32f10x.h"                  // Device header


void My_SPI_W_SS(uint8_t BitValue)  // 写SS引脚（软件模拟）
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_Init(void) // 硬件SPI 初始化
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // SS通用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    // SCK MOSI复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    // MISO上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    SPI_InitTypeDef SPI_InitStructure;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      // 双线全双工
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                       // 八位数据
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                      // 高位先行
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;    // 分频系数等于SCK频率 
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                              // 模式0 默认低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                            // 模式0 默认第一个边沿采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                               // 软件NSS
    SPI_InitStructure.SPI_CRCPolynomial = 7;                                // CRC校验位 
    SPI_Init(SPI1, &SPI_InitStructure);
    
    SPI_Cmd(SPI1, ENABLE);
    
    My_SPI_W_SS(1);     // 默认给高电平
}

void MySPI_Start(void)  // 起始条件（软件模拟）
{
    My_SPI_W_SS(0);
}

void MySPI_Stop(void)   // 终止条件（软件模拟）
{
    My_SPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)    // 硬件SPI 时序交换一个字节（非连续传输）
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);       // 等待TXE标志位=1 表示TDR为空；写入DR后自动清除
    
    SPI_I2S_SendData(SPI1, ByteSend);      // 软件写入数据ByteSend到TDR 自动转移到移位寄存器生成波形
    
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);      // 等待RXNE=1 收到一个字节表示发送时序产生完成了，读DR后清除
    
    return SPI_I2S_ReceiveData(SPI1);       // 发送同时接收
}
