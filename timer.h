void setupTimer(int interval);
double timeElapsed(void);
void timerReset(void);
double totalTimeElapsed(void);
void timerSleep(float time);

extern long totalCountOverflows;
extern long totalCount;
extern long count;
extern int interval;
