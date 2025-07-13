#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "stdarg.h"


uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[4];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // pa9作为TX口设置为复用输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;      // 波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;             // 发送接收模式
    USART_InitStructure.USART_Parity = USART_Parity_No;         // 校验位
    USART_InitStructure.USART_StopBits  = USART_StopBits_1;     // 停止位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 数据帧长度
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART1, ENABLE);


}

void Serial_SendByte(uint8_t Byte)      // 发送一个字节
{
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);   // 等待TDR数据转移到移位寄存器，TXE置1，写操作会自动清零
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)  // 发送一个数组
{
    uint16_t i;
    for (i = 0; i < Length; i ++)
    {
        Serial_SendByte(Array[i]);
    }
}

void Serial_SendString(char *String)    // 发送一个字符串
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) // 以字符串最后的结束位作为判断标志
    {
        Serial_SendByte(String[i]);     
    }
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y) // 计算X的Y次方的功能
{
    uint32_t Result = 1;
    while (Y --)
    {
        Result *= X;
    }
    return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length) // 发送一个数字,需要按照十进制拆分数字
{
    uint8_t i;
    for (i = 0; i < Length; i ++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i -1) % 10 + 0x30);    // 最终以字符的形式显示，需要加上偏移0x30或'0'
    }
}

int fputc(int ch, FILE *f)  // 移植printf函数
{
    Serial_SendByte(ch);
    return ch;
}

void Serial_SendPacket(void)
{
    Serial_SendByte(0xFF);
    Serial_SendArray(Serial_TxPacket, 4);
    Serial_SendByte(0xFE);
}

uint8_t Serial_GetRxFlag(void)
{
    if (Serial_RxFlag == 1)
    {
        Serial_RxFlag = 0;
        return 1; 
    }
    return 0;
}

void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}

void USART1_IRQHandler(void)
{
    static uint8_t RxState = 0;
    static uint8_t pRxState = 0;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        uint8_t RxData = USART_ReceiveData(USART1);

        if (RxState == 0)
        {
            if (RxData == 0xFF)
            {
                RxState = 1;
                pRxState = 0;
            }
        }
        else if (RxState == 1)
        {
            Serial_RxPacket[pRxState] = RxData;
            pRxState++;
            if (pRxState >= 4)
            {
                RxState = 2;
            }
        }
        else if (RxState == 2)
        {
            if (RxData == 0xFE)
            {
                RxState = 0;
                Serial_RxFlag = 1;
            }
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
