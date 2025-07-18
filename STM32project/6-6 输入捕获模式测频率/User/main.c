#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "OLED.h" 
#include "PWM.h"
#include "IC.h"


int main (void)
{
    OLED_Init();
    PWM_Init();
    IC_Init();
    
    OLED_ShowString(1, 1, "Freq:00000HZ");
    
    PWM_SetPerscaler(720 - 1);   // Freq = 720M / (PSC + 1)/ 100
    PWM_SetCompare1(50);       // Duty = CRR / 100

    while (1)
    {
        OLED_ShowNum(1, 6, IC_GetFreq(), 5);
    }
}
