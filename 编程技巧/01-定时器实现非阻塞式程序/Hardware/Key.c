#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint8_t Key_Num;

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if (Key_Num)
	{
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
}

/* 获取按键当前状态 按键1按下返回1 按键2按下返回2 都没有按下返回0 */
uint8_t Key_GetState(void)
{
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		return 1;
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		return 2;
	}
	return 0;
}

/* 20ms扫描按键 通过定时中断实现按键的扫描 解决按键扫描的阻塞问题 */
void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrState, PrevState;

	Count ++;
	
	if (Count >= 20)		//中断1ms进一次 进行分频实现20ms进一次
	{
		Count = 0;

		PrevState = CurrState;
		CurrState = Key_GetState();

		if (CurrState == 0 && PrevState != 0)	// 上一次扫描为0这一次扫描为1/2 判断按键按下松开 且当前处于松开状态
		{
			Key_Num = PrevState;			// Key_Num为上次按下的值；
		}
    }
}
