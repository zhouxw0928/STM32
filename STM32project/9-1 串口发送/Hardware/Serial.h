#ifndef __SERIAL_H
#define __SERIAL_H
#include "stdint.h"
#include <stdio.h>


void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length); // 发送一个字符形式的数字,需要按照十进制拆分数字
void Serial_Printf(char *format, ...);

#endif
