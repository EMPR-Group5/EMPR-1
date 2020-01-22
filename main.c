#include <stdio.h>              // Global includes
#include "lpc17xx_gpio.h"       // Central include files
#include "lpc17xx_systick.h"	
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "utils.h"              // Local functions
#include "i2c.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include "utils.h"
#include "TCS347255.h"

char output[128];
int len;

void main(void)
{
    initAll();

    // i2cScanAll();    
    timerSleep(1);

    while(1)
    {
        TCSRead();
        timerSleep(0.05);
    }
}