#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"


#define MPU6050_ADDRESS     0XD0

void MPU6050_WaiteEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)  // 封装超时等待
{
    uint32_t TimeOut;
    TimeOut = 10000;
    while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
    {
        TimeOut--;
        if (TimeOut == 0)
        {
            break;
        }
    }
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data) // 指定地址写寄存器（一个时序）
{
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaiteEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 检测EV5事件
    
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    MPU6050_WaiteEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    
    I2C_SendData(I2C2, RegAddress);
    MPU6050_WaiteEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
    
    I2C_SendData(I2C2, Data);
    MPU6050_WaiteEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED); //检测EV8-2事件
    
    I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)  // 指定地址读一个字节
{
    uint8_t Data;

    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaiteEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); // 检测EV5事件
    
    
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    MPU6050_WaiteEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    
    I2C_SendData(I2C2, RegAddress);
    MPU6050_WaiteEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
 
    I2C_GenerateSTART(I2C2, ENABLE);
    MPU6050_WaiteEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
    
    I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
    MPU6050_WaiteEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
    
    I2C_AcknowledgeConfig(I2C2, DISABLE);   //接收最后一个字节应答位置0
    I2C_GenerateSTOP(I2C2, ENABLE);         // 停止位置1 申请产生终止条件
    
    MPU6050_WaiteEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);    // 数据传输完成等待EV7时间
    
    Data = I2C_ReceiveData(I2C2);
    I2C_AcknowledgeConfig(I2C2, ENABLE);    // 接收结束ACK置1 方便多个字节接收
    
    return Data;
}

void MPU6050_Init(void)
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE) ;

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 |GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_ClockSpeed = 50000;
    I2C_InitStructure.I2C_DutyCycle =I2C_DutyCycle_2;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_Init(I2C2, &I2C_InitStructure);
    
    I2C_Cmd(I2C2, ENABLE);


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
