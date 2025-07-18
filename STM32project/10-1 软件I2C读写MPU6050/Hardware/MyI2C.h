#ifndef __MYI2C_H
#define __MYI2C_H


void MyI2C_Init(void);              // 初始化
void MyI2C_Start(void);             // 兼容起始条件和重复起始条件
void MyI2C_Stop(void);              // 结束
void MyI2C_SendByte(uint8_t Byte);   // 发送数据位
uint8_t MyI2C_RecieveByte(void);     // 接收数据位
void MyI2C_SendAck(uint8_t AckBit);  // 发送应答
uint8_t MyI2C_RecieveAck(void);      // 接收应答

#endif
