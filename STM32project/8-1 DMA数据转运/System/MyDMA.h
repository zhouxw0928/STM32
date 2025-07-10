#ifndef __MYDMA_H
#define __MYDMA_H
#include "stdint.h"


void MyDMA_Init(uint32_t AddrA, uint32_t AddrB,uint16_t Size);
void DMA_Transfer(void);


#endif
