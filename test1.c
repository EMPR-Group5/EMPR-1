#include "utils.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"	
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#include "lpc_types.h"
#include <stdio.h>
#include "i2c.h"
#include "lcd.h"
#include "timer.h"
#include "keypad.h"
#include "adc.h"
#include "dac.h"
#include "waveformer.h"
#include "pwm.h"




void main(void)
{

    Startup();
    char output[128];    
    int len;

    StartWaveformer();
    
    int freq = 100;
    float amp = 0.0;

    timerReset();

    while(1)
    {
        if(timeElapsed() > 1)
        {
            freq += 100;
            if(freq > 1000)
            {
                freq = 0;
            }
            amp += 0.05;
            if(amp > 0.5)
            {
                amp = 0;
            }
            SetWaveformFrequency(freq);
            SetWaveformAmplitude(amp);
            timerReset();
        }
        if(keypadRead() > -1)
        {
            break;
        }
    }

    StopWaveformer();
    MirrorInput(1);

    while(1)
    {
        if(keypadRead() > -1)
        {
            MirrorInput(0);
            break;
            
        }
    }

    StartPWM();

    int b;
    for(b = 0; b < 5; b++)
    {
        int a;
        for(a = 0; a<20; a++)
        {
            float dc = (float)a/20;
            //len = sprintf(output, "Setting DC: %.2f\n\r", dc);
            //serialWrite(output, len);
            SetChannelDutyCycle(1, dc);
            timerSleep(0.25);
        }
    }

    StopPWM();
    serialWrite("Finished\n\r", 12);


    /*
    while(1)
    {
        int a;
        for(a = 0; a<10; a++)
        {
            float dc = a/10;
            len = sprintf(output, "Setting DC: %.2f", dc);
            SetChannelDutyCycle(1, dc);
            timerSleep(4);
        }
    }
    */
    /*
    int freqNum;
    for(freqNum=2; freqNum<10; freqNum++)
    {
        int frequency = freqNum*1000;
        SetWaveformFrequency(frequency);
        len = sprintf(output, "Setting Frequency: %d\n\r", frequency);
        serialWrite(output, len);
        timerSleep(1);
    }
    timerSleep(2);
    float amplitude = 0;
    int amp;
    for(amp = 0; amp < 6; amp++)
    {
        amplitude = (float)amp/10;
        SetWaveform    //NVIC_EnableIRQ(TIMER0_IRQn);
    setupSerial();1

    
    
    //setupI2C();
    //setupTimer(1);
    //lcdSetup();
    
    //setupDAC();

    
    serialWrite("Starting Setup\n\r", 18);
    setupADC();
    //setupWaveformer(10000);
    serialWrite("\n\rStarting\n\r", 12);

    //setupPWM(1);
    
    1);
    }
    StopWaveformer();*/

}


void checkButton(int button, int * buffer, int * count, int bufferSize)
{
    if(button == 42)
    {
        fillArray(buffer, 32, bufferSize);
        *count = 0;
    }
    return;
}

void fillArray(int * array, int value, int length)
{
    int a = 0;
    for(a=0; a<length;a++)
    {
        array[a] = value;
    }
}

void Startup()
{
    setupSerial();
    serialWrite("\n\rSerial Setup Complete\n\r", 25);

    setupI2C();
    serialWrite("I2C Setup Complete\n\r", 22);

    setupTimer(1);
    serialWrite("Timer Setup Complete\n\r", 24);

    lcdSetup();
    serialWrite("LCD Setup Complete\n\r", 22);

    setupDAC();
    serialWrite("DAC Setup Complete\n\r", 22);

    setupADC();
    serialWrite("ADC Setup Complete\n\r", 22);

    setupWaveformer(100);
    serialWrite("Waveformer Setup Complete\n\r", 28);

    setupPWM(1);
    serialWrite("PWM Setup Complete\n\r", 22);

    serialWrite("\n\rSetup Complete", 18);
    serialWrite("\n\r", 2);
}
