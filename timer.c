#include "lpc17xx_systick.h"

long totalCountOverflows;
long long totalCount;
long count;

void setupTimer()
{
    count = 0;
    SYSTICK_InternalInit(1);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);
}

double timeElapsed(void)
{
    double time = count * 0.001;
    return(time);
}

double totalTimeElapsed(void)
{
    double time = totalCount * 0.001;
    return(time);
}

void timerReset(void)
{
    count = 0;
}

void timerSleep(int duration)
{
    timerReset();
    while(timeElapsed() < 1){}
}

void SysTick_Handler(void)
{
    count += 1;
    totalCount += 1;
    if(~totalCount == 0)
    {
        totalCountOverflows += 1;
    }
}