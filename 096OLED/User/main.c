#include "stm32f10x.h"                  // Device header
#include "OLED.h"


int main(void)
{
    
    OLED_Init();
    OLED_ShowImage(40, 0, 60, 90, OldMan);
    OLED_Update();
    
    while(1)
    {
        ;
    }
}
