#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"


#define MPU6050_ADDRESS     0XD0

// 进行寄存器写操作之前要先解除睡眠模式 寄存器0x68写入0x00;
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data) // 指定地址写寄存器（一个时序）
{
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_RecieveAck();     // 接收应答位
    MyI2C_SendByte(RegAddress);
    MyI2C_RecieveAck(); 
    MyI2C_SendByte(Data);
    MyI2C_RecieveAck(); 
    MyI2C_Stop(); 
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)  // 指定地址读一个字节
{
    uint8_t Data;
    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS);
    MyI2C_RecieveAck();     // 接收应答位
    MyI2C_SendByte(RegAddress);
    MyI2C_RecieveAck(); 

    MyI2C_Start();
    MyI2C_SendByte(MPU6050_ADDRESS | 0X01); // 0X01读写位为1，为读指令
    MyI2C_RecieveAck(); 
    Data = MyI2C_RecieveByte();
    MyI2C_SendAck(1);   // 从机发送应答，1为不继续读，0为继续读
    MyI2C_Stop(); 

    return Data;
}

void MPU6050_Init(void)
{
    MyI2C_Init();
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);     // 解除睡眠 使用陀螺仪时钟
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);     // 6个轴均不待机
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09); // 10分频
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);     // 滤波参数给最大
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);    // 陀螺仪选择最大量程
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);   // 加速度计选择最大量程
}

uint8_t MPU6050_GetID(void) //获取ID
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

//实现多返回值函数。 1 写六个全局变量；2 用指针进行变量地址传递；3 用结构体对多个变量进行打包统一进行传递
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
                        int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)  //实现多返回值函数
{
    uint8_t DataH, DataL;
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);   //读取加速度计X轴高八位
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L); //读取加速度计X轴低八位
    *AccX = (DataH << 8) | DataL; //获取16位数据，并通过指针传递

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H); 
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L); 
    *AccZ = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);   //读取陀螺仪计X轴高八位
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);   //读取陀螺仪计X轴低八位
    *GyroX = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (DataH << 8) | DataL;

    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (DataH << 8) | DataL;
}
