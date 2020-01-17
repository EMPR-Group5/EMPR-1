#include "lpc17xx_systick.h"

long totalCountOverflows;
long long totalCount;
long count;    
int interval;

void setupTimer(int inter)
{
    interval = inter;
    count = 0;
    SYSTICK_InternalInit(inter);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);
}

double timeElapsed(void)
{
    double time = count*interval*0.001;
    return(time);
}

double totalTimeElapsed(void)
{
    //long z = ~0 ;
    //double time = ((totalCountOverflows+1)*z)*totalCount*interval*0.001;
    double time = totalCount * interval * 0.001;
    return(time);
}

void timerReset(void)
{
    count = 0;
}

void timerSleep(float time)
{
    double startTime = timeElapsed();
    while(timeElapsed() - startTime < time)
    {}
    return;
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