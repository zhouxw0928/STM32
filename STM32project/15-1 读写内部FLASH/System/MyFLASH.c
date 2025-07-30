#include "stm32f10x.h"                  // Device header


uint32_t MyFLASH_ReadWord(uint32_t Address)     // 读取字
{
    return *((__IO uint32_t *)(Address));   // 返回地址内容
}

uint16_t MyFLASH_ReadHalfWord(uint32_t Address)     // 读取半字
{
    return *((__IO uint16_t *)(Address));
}

uint8_t MyFLASH_ReadByte(uint32_t Address)      // 读取字节
{
    return *((__IO uint8_t *)(Address));
}

void MyFLASH_EraseALLPages(void)    // 全擦除
{
    FLASH_Unlock();             // 解锁
    FLASH_EraseAllPages();      // 全擦除
    FLASH_Lock();               // 擦除完锁上
}    

void MyFLASH_ErasePage(uint32_t PageAddress)
{
    FLASH_Unlock();             // 解锁
    FLASH_ErasePage(PageAddress);
    FLASH_Lock();               // 擦除完锁上
}

void MyFLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Unlock();             // 解锁
    FLASH_ProgramWord(Address, Data);
    FLASH_Lock();               // 擦除完锁上
}

void MyFLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Unlock();             // 解锁
    FLASH_ProgramHalfWord(Address, Data);
    FLASH_Lock();               // 擦除完锁上
}
