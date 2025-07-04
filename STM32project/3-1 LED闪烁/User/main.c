#include "stm32f10x.h"                  // Device header
#include "Delay.h" 


int main (void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;                // 定义结构体
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // 工作模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           // 定义引脚位置。0号引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // GPIO读写函数
//    uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//    uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
//    uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//    uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
//    void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);    // 指定引脚为高电平
//    void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);      // 指定引脚为低电平
//    void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal); // 根据第三个参数的值指定端口
//    void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);           // 根据PortVal同时对16个端口进行写入
    
    //实际操作
    GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET); 
    
    while (1)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        Delay_ms(500);      //毫秒延时，500ms
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        Delay_ms(500);
        

        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);    // 点亮LED
        Delay_ms(500);      //毫秒延时，500ms
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET); 
        Delay_ms(500);
        
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);    // 点亮LED
        Delay_ms(500);      //毫秒延时，500ms
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)1); 
        Delay_ms(500);
    }
}
