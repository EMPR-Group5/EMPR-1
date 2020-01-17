#include "dac.h"
#include "lpc17xx_timer.h"
#include "lpc_types.h"
#include "utils.h"
#include <stdio.h>

int sinWave[10] = {0,601,973,974,602,0,-600,-973,-974,-604};
//int sinWave[2] = {-1000, 1000};
int waveformLength = 10;

float waveformScale = 0.5;
int waveformBias = 0.5*1024;
int waveformCounter = 0;

int clock = 100000000;

void setupWaveformer(int freq)
{
    int matchCount = GetMatchValue(freq);
    int reg = 0;

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

    //Stop Waveformer
    StopWaveformer();
}

int GetMatchValue(int freq)
{
    int matchCount = (int) (clock/(freq*100*waveformLength));
    return matchCount;
}

void SetWaveformFrequency(int freq)
{
    //setupWaveformer(freq);/10
    TIM_UpdateMatchValue(LPC_TIM0, 0, GetMatchValue(freq));
    TIM_ResetCounter(LPC_TIM0);
}

void SetWaveformAmplitude(float amplitudeScale)
{
    waveformScale = amplitudeScale;
}

void incrementCounter()
{
    waveformCounter += 1;
    if(waveformCounter >= waveformLength)
    {
        waveformCounter = 0;
    }
    int dacValue = convertValue(sinWave[waveformCounter]);
    updateDAC(dacValue);
}

int convertValue(int value)
{
    float val = value * waveformScale;

    return val + waveformBias;
}

void StopWaveformer()
{
    updateDAC(0);
    TIM_Cmd(LPC_TIM0, DISABLE);
}

void StartWaveformer()
{
    updateDAC(0);
    TIM_Cmd(LPC_TIM0, ENABLE);
}

void TIMER0_IRQHandler(void)
{
    
    incrementCounter();
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}