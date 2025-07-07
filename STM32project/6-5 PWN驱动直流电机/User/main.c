#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h" 
#include "Motor.h"
#include "Key.h"


uint8_t KeyNum;
int8_t Speed;       // 这样的定义默认Speed为0;

int main (void)
{
    OLED_Init();
    Motor_Init();
    Key_Init();
    
    OLED_ShowString(1, 1, "Speed:");

    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            Speed += 10;
            if (Speed > 50)
            {
                Speed = -50;
            }
        }
        Motor_SetSpeed(Speed);
        OLED_ShowSignedNum(1, 7, Speed, 3);
    }
}
