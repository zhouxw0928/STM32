#include "stm32f10x.h"                  // Device header
#include "Key.h"

#define KEY_PRESSED			1
#define KEY_UNPRESSED		0

#define KEY_TIME_DOUBLE		200		// 两次按下的时间间隔
#define KEY_TIME_LONG		2000	// 按下多久算常按
#define KEY_TIME_REPEAT		100		// 长按后多久数字+1

#define KEY_COUNT			4

uint8_t Key_Flag[KEY_COUNT];

void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/* 获取按键当前状态 按键按下返回Key_PRESSED 松开未按下Key_UNPRESSED */
uint8_t Key_GetState(uint8_t n)
{
	if (n == KEY_1)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{
			return KEY_PRESSED;
		}
	}
	else if (n == KEY_2)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
		{
			return KEY_PRESSED;
		}
	}
	else if (n == KEY_3)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 1)
		{
			return KEY_PRESSED;
		}
	}
	else if (n == KEY_4)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 1)
		{
			return KEY_PRESSED;
		}
	}
	return KEY_UNPRESSED;
}

/* 读取标志位 变量&位掩码 结果非0该位为1 */
uint8_t Key_Check(uint8_t n, uint8_t Flag)
{
	if (Key_Flag[n] & Flag)	
	{
		if (Flag != KEY_HOLD)	// HOLD状态不用清零
		{
			Key_Flag[n] &= ~Flag;	// 给指定标志位清零
		}
		return 1;
	}
	return 0;
}

/* 状态机编程思路实现按键的4种不同功能 */
void Key_Tick(void)
{
	static uint8_t Count, i;
	static uint8_t CurrState[KEY_COUNT], PrevState[KEY_COUNT];
	static uint8_t S[KEY_COUNT];
	static uint16_t Time[KEY_COUNT];	// 设定长按时间

	for (i = 0; i < KEY_COUNT; i ++)
	{
		if (Time > 0)
		{
			Time[i]--;
		}
	}
	
	Count ++;
	
	if (Count >= 20)		//中断1ms进一次 进行分频实现20ms进一次
	{
		Count = 0;

		for (i = 0; i < KEY_COUNT; i++)
		{
			PrevState[i] = CurrState[i];
			CurrState[i] = Key_GetState(i);

			if (CurrState[i] == 1)		// 按下状态 HOLD=1
			{
				Key_Flag[i] |= KEY_HOLD;
			}
			else					
			{
				Key_Flag[i] &= ~KEY_HOLD;				//未按下状态 HOLD =0
			}
			if (CurrState[i] == KEY_PRESSED && PrevState[i] == KEY_UNPRESSED)		// 按键按下状态 Down=1 标志位由主程序读取后清零
			{
				Key_Flag[i] |= KEY_DOWN;
			}
			if (CurrState[i] == KEY_UNPRESSED && PrevState[i] == KEY_PRESSED)		// 按键松开状态	UP=1
			{
				Key_Flag[i] |= KEY_UP;			// 标志位位掩码
			}

			if (S[i] == 0)
			{
				if (CurrState[i] == KEY_PRESSED)
				{
					Time[i] = KEY_TIME_LONG;			//设置长按时间数值2000
					S[i] = 1;
				}
			}
			else if (S[i] == 1)
			{
				if (CurrState[i] == KEY_UNPRESSED)
				{
					Time[i] = KEY_TIME_DOUBLE;			// 设定双击时间200ms
					S[i] = 2;                          // 进入状态2 
				}
				else if (Time[i] == 0)
				{
					Time[i] = KEY_TIME_REPEAT;			// 设置重复时间
					Key_Flag[i] |= KEY_LONG;			// LONG=1;
					S[i] = 4;
				}
			}
			else if (S[i] == 2)
			{
				if (CurrState[i] == KEY_PRESSED)
				{
					Key_Flag[i] |= KEY_DOUBLE;			// Double = 1;
					S[i] = 3;
				}
				else if (Time[i] == 0)
				{
					Key_Flag[i] |= KEY_SINGLE;			// Sigle =1;
					S[i] = 0;
				}
				
			}
			else if (S[i] == 3)
			{
				if (CurrState[i] == KEY_UNPRESSED)
				{
					S[i] = 0;
				}
			}
			else if (S[i] == 4)
			{
				if (CurrState[i] == KEY_UNPRESSED)
				{
					S[i] = 0;
				}
				else if (Time[i] == 0)
				{
					Time[i] = KEY_TIME_REPEAT;		//重置重复时间
					Key_Flag[i] |= KEY_REPEAT;		// REPEAT = 1;
					S[i] = 4;
				}
			}
			}	
    }
}
