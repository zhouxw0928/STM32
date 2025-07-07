#include "stm32f10x.h"                  // Device header
#include "PWM.h"


void Servo_Init(void)
{
    PWM_Init();
}

// 0度对应500，180°对应2500
void Servo_SetAngle(float Angle)
{
    PWM_SetCompare2(Angle / 180 * 2000 + 500);
    if (Angle < 0)
    {
        Angle = 0;
    }
    if (Angle > 180)
    {
        Angle = 180;
    }
}
