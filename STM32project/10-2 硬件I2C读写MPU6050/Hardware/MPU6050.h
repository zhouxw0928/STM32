#ifndef __MPU6050_H
#define __MPU6050_H
#include"stdio.h"


void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data); // 指定地址写寄存器（一个时序）
uint8_t MPU6050_ReadReg(uint8_t RegAddress);  // 指定地址读一个字节

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void); //获取ID

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
                        int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);

#endif
