#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h"


#define STORE_STRART_ADDRESS    0x0800FC00
#define STORE_COUNT             512

uint16_t Store_Data[STORE_COUNT];

void Store_Init(void)   // 第一次使用给各个参数都置0
{
    if (MyFLASH_ReadHalfWord(STORE_STRART_ADDRESS) != 0xA5A5) // 自己定义的标志位 不是A5A5即第一次使用
    {
        MyFLASH_ErasePage(STORE_STRART_ADDRESS);      // 擦最后一页
        MyFLASH_ProgramHalfWord(STORE_STRART_ADDRESS, 0xa5a5);    // 写入标志位
        for (uint16_t i = 1; i < STORE_COUNT; i++)  
        {
            MyFLASH_ProgramHalfWord(STORE_STRART_ADDRESS + i * 2, 0x0000);    // 除标志位其他位写0
        }
    }
    /*上电时，将闪存数据加载回SRAM数组，实现SRAM数组的掉电不丢失*/
    for (uint16_t i = 0; i < STORE_COUNT; i++)
    {
        Store_Data[i] = MyFLASH_ReadHalfWord(STORE_STRART_ADDRESS + i * 2);   //将闪存的数据加载回SRAM数组
    }
}

void Store_Save(void)
{
    MyFLASH_ErasePage(STORE_STRART_ADDRESS);  // 擦除最后一页
    for (uint16_t i = 0; i < STORE_COUNT; i++)  // 数组所有内容备份到闪存最后一页
    {
        MyFLASH_ProgramHalfWord(STORE_STRART_ADDRESS + i * 2, Store_Data[i]);
    }
}

void Store_Clear(void)
{
    for (uint16_t i = 1; i < STORE_COUNT; i++)  // 数组所有内容备份到闪存最后一页
    {
        Store_Data[i] = 0;
    }
    Store_Save();
}
