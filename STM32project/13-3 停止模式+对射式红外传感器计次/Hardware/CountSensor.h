#ifndef __COUNTERSENSOR_H
#define __COUNTERSENSOR_H


// 中断函数不需要声明，是自动执行的
void CountSensor_Init(void);
uint16_t CountSensor_Get(void);

#endif
