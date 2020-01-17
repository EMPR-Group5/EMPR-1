#include "LPC17xx.h"
#include "lpc17xx_pwm.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "utils.h"

int pwmPeriod = 100000/2;


void setupPWM(channel)
{

    char output[64];
    int len;

    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 1;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 2;
    PinCfg.Portnum = 2;
    PinCfg.Pinnum = channel-1;
    PINSEL_ConfigPin(&PinCfg);


    //Config Match
    //CONFIG MATCH RESGISTER 0
    PWM_MATCHCFG_Type MR0Config;
    MR0Config.MatchChannel = 0;

    PWM_ConfigMatch(LPC_PWM1, &MR0Config);
    PWM_MatchUpdate(LPC_PWM1, 0, pwmPeriod, PWM_MATCH_UPDATE_NOW);

    //CONFIG MATCH RESGISTER X
    PWM_MATCHCFG_Type matchConfig;
    matchConfig.MatchChannel = channel;
    PWM_ConfigMatch(LPC_PWM1, &matchConfig);
    //Set Match Value
    PWM_MatchUpdate(LPC_PWM1, channel, (int)(pwmPeriod/2), PWM_MATCH_UPDATE_NOW);
 
    PWM_TIMERCFG_Type pwmConfig;
    pwmConfig.PrescaleValue = 1;
    pwmConfig.PrescaleOption = PWM_TIMER_PRESCALE_TICKVAL;
    //PWM_ConfigStructInit(PWM_MODE_TIMER, &pwmConfig);
    PWM_Init(LPC_PWM1, PWM_MODE_TIMER, &pwmConfig);

    
    PWM_CounterCmd(LPC_PWM1, ENABLE);
    PWM_Cmd(LPC_PWM1, ENABLE);
    PWM_ChannelCmd(LPC_PWM1, channel, ENABLE);
    

    NVIC_EnableIRQ(PWM1_IRQn);
    __enable_irq();

    PWM_ResetCounter(LPC_PWM1);
    StopPWM();
    //DEBUG PWM STUFF
    /*
    int * MR = 0x40018018;
    
    len = sprintf(output, "MR0: %x\n\r", *MR);
    serialWrite(output, len);

    //MR1
    MR = 0x4001801C;
    len = sprintf(output, "MR1: %x\n\r", *MR);
    serialWrite(output, len);

    MR = 0x40018014;
    len = sprintf(output, "MCR: %x\n\r", *MR);
    serialWrite(output, len);

    MR = 0x4001804C;
    //*(int*)0x4001804C = 0x100;
    len = sprintf(output, "PWM CR: %x\n\r", *MR);
    serialWrite(output, len);

    MR = 0x40018004;
    len = sprintf(output, "PWM TCR: %x\n\r", *MR);
    serialWrite(output, len);

    MR = 0x4001800C;
    len = sprintf(output, "PWM PR: %x\n\r", *MR);
    serialWrite(output, len);

    MR = 0x40018050;
    *(int*)0x40018050 = 0x3;
    len = sprintf(output, "PWM LER: %x\n\r", *MR);
    serialWrite(output, len);

    serialWrite("PWM Init Configed TEST\n\r", 25);

    //Config Timer
    MR = 0x40018008;
    len = sprintf(output, "PWM TC: %x\n\r", *MR);
    serialWrite(output, len);
    */



    //SetChannelDutyCycle(channel, 0.5f);
    //PWM_ResetCounter(LPC_PWM1);


}

void StartPWM()
{
    PWM_Cmd(LPC_PWM1, ENABLE);
}

void StopPWM()
{
    PWM_Cmd(LPC_PWM1, DISABLE);
}

void SetChannelDutyCycle(int channel, float dutyCycle)
{

    int matchValue = pwmPeriod*dutyCycle;

    //char output[64];
    //int len = sprintf(output, "matchValue: %d, channel: %d\n\r", matchValue, channel);
    //serialWrite(output, len);

    PWM_MatchUpdate(LPC_PWM1, channel, matchValue, PWM_MATCH_UPDATE_NOW);
}

int CheckPwmFlag()
{

    if(PWM_GetIntStatus(LPC_PWM1, PWM_INTSTAT_MR0) == SET)
    {
        return 1;
    }
    if(PWM_GetIntStatus(LPC_PWM1, PWM_INTSTAT_MR1) == SET)
    {
        return 1;
    }
    return 0;
}

void PWM1_IRQHandler(void)
{
    serialWrite("PWM Interrupt\n\r", 17);
}