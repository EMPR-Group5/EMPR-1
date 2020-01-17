#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "dac.h"
#include "utils.h"
#include "lpc17xx_timer.h"


void setupADC()
{
    //Configure pins for ADC4 and ADC5
    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 24;
	PINSEL_ConfigPin(&PinCfg);
	//PinCfg.Pinnum = 31;
	//PINSEL_ConfigPin(&PinCfg);

    //Start ADC at 200KHz
    ADC_Init(LPC_ADC, 200000);
	//Enable channel 1
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_1, ENABLE);
	//Set to read continuously
	ADC_StartCmd(LPC_ADC, ADC_START_CONTINUOUS);
	//Start reading
	ADC_BurstCmd(LPC_ADC, ENABLE);

	//Enable Interrupts for channel 1
	ADC_IntConfig(LPC_ADC, ADC_ADINTEN1, SET);

}

void MirrorInput(int enable)
{
	if(enable == 1)
	{

		int matchCount = 20;
		TIM_MATCHCFG_Type matchConfig;
		matchConfig.MatchChannel = 0;
		matchConfig.IntOnMatch = ENABLE;
		matchConfig.ResetOnMatch = ENABLE;
		matchConfig.StopOnMatch = DISABLE;
		matchConfig.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;

		//Setup Match Register
		TIM_ConfigMatch(LPC_TIM1, &matchConfig);
		TIM_UpdateMatchValue(LPC_TIM1, 0, matchCount);

		//Setup Timer
		TIM_TIMERCFG_Type timerConfig;
		TIM_ConfigStructInit(TIM_TIMER_MODE, &timerConfig);

		//Start Timer
		TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &timerConfig);
		TIM_Cmd(LPC_TIM1, ENABLE);

		//Enable Timer Interrupts
		NVIC_EnableIRQ(TIMER1_IRQn);
		__enable_irq();
	}
	else
	{
		TIM_Cmd(LPC_TIM1, DISABLE);
		//NVIC_DisableIRQ(TIMER1_IRQn);
		//__enable_irq();
		updateDAC(0);
	}
}


int readADCRaw(int channel)
{
	return ADC_ChannelGetData(LPC_ADC, channel);
}

float readADC(int channel)
{
	int val = ADC_ChannelGetData(LPC_ADC, channel);
	float voltage = (3.3/0x0fff) * val;
	return voltage;
}


void TIMER1_IRQHandler(void)
{
	int readValue = ADC_GetData(1);
	updateDAC(readValue >> 2);
	TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);
}


void ADC_IRQHandler(void)
{
	int readValue = ADC_GetData(1);
	updateDAC(readValue >> 2);

}
