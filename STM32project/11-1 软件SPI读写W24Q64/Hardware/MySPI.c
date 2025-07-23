#include "stm32f10x.h"                  // Device header


void My_SPI_W_SS(uint8_t BitValue)  // 写SS引脚
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void My_SPI_W_SCK(uint8_t BitValue)  // 写时钟引脚
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

void My_SPI_W_MOSI(uint8_t BitValue)  // 写主机输出引脚
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

uint8_t MySPI_R_MISO(void)          // 读从机输入
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

void MySPI_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // 输出引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     // 输入引脚为上拉输入
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    My_SPI_W_SS(1); // 默认不选中从机
    My_SPI_W_SCK(0); // 默认使用模式0
}

void MySPI_Start(void)  // 起始条件
{
    My_SPI_W_SS(0);
}

void MySPI_Stop(void)   // 终止条件
{
    My_SPI_W_SS(1);
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)    // 交换一个字节
{
    uint8_t i, ByteReceive = 0x00;
    
    for (i = 0; i < 8; i++) // 循环八次 交换一个字节
    {
        My_SPI_W_MOSI(ByteSend & (0x80 >> i)); // 移出最高位
        My_SPI_W_SCK(1);    // 写SCK产生上升沿 
        if (MySPI_R_MISO() == 1){ByteReceive |= (0x80 >> i);}
        My_SPI_W_SCK(0);    // 写SCK产生下降沿  
    }
    return ByteReceive; 
}

//uint8_t MySPI_SwapByte(uint8_t ByteSend)    // 移位模型实现 交换一个字节
//{
//    uint8_t i;
//    
//    for (i = 0; i < 8; i++) // 循环八次 交换一个字节
//    {
//        My_SPI_W_MOSI(ByteSend & 0x80); // 移出最高位
//        ByteSend <<= 1;
//        My_SPI_W_SCK(1);    // 写SCK产生上升沿 
//        if (MySPI_R_MISO() == 1){ByteSend |= 0x01;}
//        My_SPI_W_SCK(0);    // 写SCK产生下降沿  
//    }
//    return ByteSend; 
//}
