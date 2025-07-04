#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "BUZZER.h"
#include "LightSensor.h"

uint8_t KeyNum;

int main (void)
{
    Buzzer_Init();
    
    while (1)
    { 
//        Buzzer_ON();
//        Delay_ms(500);
//        Buzzer_OFF();
//        Delay_ms(500);
//        Buzzer_Turn();
//        Delay_ms(500);
//        Buzzer_Turn();
//        Delay_ms(500);
//        
        if (LightSensor_Get() == 1)
        {
            Buzzer_ON();
        }
        else
        {
            Buzzer_OFF();
        }

    }
}
