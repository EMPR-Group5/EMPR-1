#include "lpc17xx_dac.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"


void setupDAC()
{
    //Configure pins for ADC4 and ADC5
    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 2;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 26;
	PINSEL_ConfigPin(&PinCfg);
	//PinCfg.Pinnum = 31;
	//PINSEL_ConfigPin(&PinCfg);


    //Init DAC
    DAC_Init(LPC_DAC);
}

void updateDAC(int value)
{
    DAC_UpdateValue(LPC_DAC, value);
}