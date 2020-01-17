void setupTimer(int interval);
double timeElapsed(void);
double totalTimeElapsed(void);
void timerReset(void);
void timerSleep(int duration);
void SysTick_Handler(void);

extern long totalCountOverflows;
extern long long totalCount;
extern long count;