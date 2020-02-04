#include "i2c.h"
#include "timer.h"
#include "lpc17xx_timer.h"
#include <math.h>
#include "utils.h"

//Motor Control Stuff
int sequence[4] = {0x09,0x05, 0x06, 0x0a};
int motorAddresses[3] = {62, 62, 63};
int motorNibbleOffset[3] = {4, 0, 0};
int currentSteps [3] = {0,0,0};
int topLimits[3] = {800, 800, 1000};
double lastMove[3] = {0,0,0};
int data[1];



//Useful Stuff
int coordinates[3] = {0,0,0};
int targetCoordinates[3] = {0,0,0};
int home[3] = {250, 0, 300};
int targetReached[3] = {0,0,0};
int paused = 0;


double movementSpeed = 0.001; 

void MotorSetup(void)
{
    SetupTimer(100);
    Calibrate();
}

void UpdateMotors()
{
    
    if(paused != 1)
    {
        //serialWrite("MotorUpdate\n\r", 15);
        int axis;
        for(axis = 0; axis<3; axis++)
        {
            MoveToTarget(axis);
        }
    }
    
}


void MoveForward(int axis)
{
    int lim = CheckLimits(axis);
    if((lim == 0) || (lim == -1))
    {
        //serialWrite("Moving Forward\n\r", 18);
        lastMove[axis] = totalTimeElapsed();
        currentSteps[axis] = CheckBounds(currentSteps[axis] + 1);

        i2cRead(motorAddresses[axis], data, 1);
        char temp = ~(0xf << motorNibbleOffset[axis]);
        data[0] = data[0] & temp;
        data[0] = data[0] | (sequence[currentSteps[axis]]<<motorNibbleOffset[axis]);


        i2cWrite(motorAddresses[axis], data, 1);
        coordinates[axis] ++;
    }
}

void MoveBack(int axis)
{
    int lim = CheckLimits(axis);
    if((lim == 0) || (lim == 1))
    {
        lastMove[axis] = totalTimeElapsed();
        currentSteps[axis] = CheckBounds(currentSteps[axis] - 1);

        i2cRead(motorAddresses[axis], data, 1);
        char temp = ~(0xf << motorNibbleOffset[axis]);
        data[0] = data[0] & temp;
        data[0] = (data[0] | (sequence[currentSteps[axis]]<<motorNibbleOffset[axis]));

        i2cWrite(motorAddresses[axis], data, 1);
        coordinates[axis] --;
    }
}

void MoveToTarget(int axis)
{
    if(targetCoordinates[axis] > coordinates[axis])
    {
        targetReached[axis] = 0;
        MoveForward(axis);
    }
    else if(targetCoordinates[axis] < coordinates[axis])
    {    
        targetReached[axis] = 0;
        MoveBack(axis);
    }
    else if(targetCoordinates[axis] == coordinates[axis])
    {
        //len = sprintf(output, "Idle %d\n\r", axis);
        //serialWrite("idle\n\r", 8);
        if(targetReached[axis] == 0)
        {
            SetIdle(axis);
        }
        targetReached[axis] = 1;
    }
    
}

void SetSingleCoordinate(int axis, int value)
{
    if((value <=  topLimits[axis]) && (value >= 0))
    {
    targetCoordinates[axis] = value;
    targetReached[axis] = 0;
    }
}

void SetCoordinates(int* coords)
{
    int axis;
    for(axis = 0; axis<3; axis++)
    {
        SetSingleCoordinate(axis, coords[axis]);
        //targetCoordinates[axis] = coords[axis];
        //targetReached[axis] = 0;
    }
}

void SetIdle(axis)
{
    i2cRead(motorAddresses[axis], data, 1);

    char temp = ~(0xf << motorNibbleOffset[axis]);

    data[0] = temp & data[0];
    i2cWrite(motorAddresses[axis], data, 1);
}

void Calibrate()
{
    paused = 1;
    while(CheckLimits(0) == 0)
    {
        timerSleep(0.001);
        MoveBack(0);
    }
    coordinates[0] = 0;

    while(CheckLimits(1) == 0)
    {
        timerSleep(0.001);
        MoveBack(1);
    }
    coordinates[1] = 0;

    while(CheckLimits(2) == 0)
    {
        timerSleep(0.001);
        MoveBack(2);
    }   
    coordinates[2] = 0;

    serialWrite("Calibrated\n\r");

    int axis;
    for(axis = 0; axis<3; axis++)
    {
        SetSingleCoordinate(axis, home[axis]);
    }

    paused = 0;
    
    while(TargetReached() == 0)
    {
        timerSleep(0.2);
        if(TargetReached() == 1)
        {
            break;
        }
    }

    for(axis = 0; axis<3; axis++)
    {
        coordinates[axis] = 0;
        targetCoordinates[axis] = 0;
    }
}

int TargetReached()
{
    if(targetReached[0] & targetReached[1] & targetReached[2])
    {
        return(1);
    }
    else
    {
        return(0);
    }
}

int CheckLimits(axis)
{
    //Return 0 if okay
    //Return 1 if at top limit
    //Return -1 if at bottom limit

    int limitStatus[1]; 
    data[0] = 0xff;

    i2cReadWrite(60, data, 1, limitStatus, 1);

    if(coordinates[axis] >= topLimits[axis])
    {
        return 1;
    }

    else if ((1<<axis) & (limitStatus[0]))
    {
        return 0;
    }

    else
    {
        return -1;
    }   
}

int CheckBounds(int value)
{
    if(value > 3)
    {
        return 0;
    }
    if(value < 0)
    {
        return 3;
    }
    return value;
}




void SetupTimer(int freq)
{
    int clock = 100000000;
    int matchCount = (int) (clock/(freq*100*2));
    matchCount = 2000;

    TIM_MATCHCFG_Type matchConfig;
    matchConfig.MatchChannel = 0;
    matchConfig.IntOnMatch = ENABLE;
    matchConfig.ResetOnMatch = ENABLE;
    matchConfig.StopOnMatch = DISABLE;
    matchConfig.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;

    //Setup Match Register
    TIM_ConfigMatch(LPC_TIM0, &matchConfig);
    TIM_UpdateMatchValue(LPC_TIM0, 0, matchCount);

    //Setup Timer
    TIM_TIMERCFG_Type timerConfig;
    TIM_ConfigStructInit(TIM_TIMER_MODE, &timerConfig);

    //Start Timer
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerConfig);
    TIM_Cmd(LPC_TIM0, ENABLE);

    //Enable Timer Interrupts
    NVIC_EnableIRQ(TIMER0_IRQn);
    __enable_irq();
}

void TIMER0_IRQHandler(void)
{
    UpdateMotors();
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}



