#include "stm32f10x.h"                  // Device header
#include <time.h>


uint16_t MyRTC_Time[] = {2025, 7, 25, 11, 12, 5};

void MyRTC_SetTime(void);

void MyRTC_Init(void)
{
    // 开启PWR RTC时钟 使能BKP RTC访问
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xa5a5)  // 通过写入BKP_DR1检查是否完全断电 只有完全断电才进行初始化重置时间
    {
        // 启动RTC时钟 开启LSE时钟 等待晶振启动标志位
        RCC_LSEConfig(RCC_LSE_ON);
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
        
        // 配置RTCCLK数据选择器
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);
        
        // 等待RSF标志位 配置CNF位 等待上次写操作结束
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
        
        // 配置预分频器输出1HZ
        RTC_SetPrescaler(32768-1);  // SET写操作自带进入和退出配置模式
        RTC_WaitForLastTask();  // 等待写操作完成
        
        // 配置CNT值 设定初始时间
    //    RTC_SetCounter(1672588795);     // 预设时间 2023-1-1 23:59:55(北京)
    //    RTC_WaitForLastTask();
        MyRTC_SetTime();
        
        BKP_WriteBackupRegister(BKP_DR1, 0xa5a5);   // 写入BKP当做标志位
    }
    else
    {
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
    }
}

void MyRTC_SetTime(void)       // 日期时间到CNT秒的转换
{
    time_t time_cnt;
    struct tm time_date;
    
    // 将数组指定时间填充到结构体里
    time_date.tm_year = MyRTC_Time[0] - 1900;
    time_date.tm_mon  = MyRTC_Time[1] - 1;
    time_date.tm_mday = MyRTC_Time[2];
    time_date.tm_hour = MyRTC_Time[3];
    time_date.tm_min  = MyRTC_Time[4];
    time_date.tm_sec  = MyRTC_Time[5];
     
    time_cnt = mktime(&time_date) - 8 * 60 * 60;    // 东八区时间 减8小时CNT计数
    
    RTC_SetCounter(time_cnt);   // 将time_cnt转化为时间
    RTC_WaitForLastTask(); 
}

void MyRTC_ReadTime(void)
{
    time_t time_cnt;
    struct tm time_date;
    
    time_cnt = RTC_GetCounter() + 8 * 60 * 60;   // 东八区时间 加8小时CNT计数
    
    time_date = *localtime(&time_cnt);
    
    MyRTC_Time[0] = time_date.tm_year + 1900;
    MyRTC_Time[1] = time_date.tm_mon + 1;
    MyRTC_Time[2] = time_date.tm_mday;
    MyRTC_Time[3] = time_date.tm_hour;
    MyRTC_Time[4] = time_date.tm_min;
    MyRTC_Time[5] = time_date.tm_sec;
}
