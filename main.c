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
#include "motorController.h"
#include "scanner.h"

char output[128];
int len;
int resolution = 10;



int lastScreenUpdate = 0;
int lastInputUpdate = 0;
int lastReadUpdate = 0;


void main(void)
{
    char* imageArray = (char*) malloc(resolution*resolution*3*sizeof(char));
    initAll();

    timerSleep(1);
    //StartScan(0, resolution, imageArray);

    serialWrite("Ready");
    timerSleep(1);
    

    while(1)
    {
        if(finished == 0)
        {
            UpdateScan();
        }
        else
        {
            if(totalTimeElapsed() > lastScreenUpdate + 1)
            {     
                lastScreenUpdate = totalTimeElapsed(); 
                UpdateScreen();
            }
            if(totalTimeElapsed() > lastInputUpdate + 0.1)
            {
                if(TargetReached() == 1)
                {
                    lastInputUpdate = totalTimeElapsed(); 
                    CheckKeypad();
                }
            }
            if(totalTimeElapsed() > lastReadUpdate + 1)
            {
                lastReadUpdate = totalTimeElapsed();
                int data[3] = {0};

                TCSRead(data);
                sprintf(output, "    %d, %d, %d, %d", data[0], data[1], data[2], data[3]);
                serialWrite(output);
                //TCSReadConverted(rgbValues);
            }
        }
    }
}

void CheckKeypad()
{
    int num = keypadRead();
    int increment = 50;
    if(num == '1')
    {
        SetSingleCoordinate(0, targetCoordinates[0] + increment);
    }
    if(num == '4')
    {
        SetSingleCoordinate(0, targetCoordinates[0] - increment);
    }

    if(num == '2')
    {
        SetSingleCoordinate(1, targetCoordinates[1] + increment);
    }
    if(num == '5')
    {
        SetSingleCoordinate(1, targetCoordinates[1] - increment);
    }
    if(num == '3')
    {
        SetSingleCoordinate(2, targetCoordinates[2] + increment);
    }
    if(num == '6')
    {
        SetSingleCoordinate(2, targetCoordinates[2] - increment);
    }
}

void UpdateScreen()
{
    lcdSetCursor(40);
    len = sprintf(output, " %d, %d, %d   ", coordinates[0], coordinates[1], coordinates[2]);
    lcdWrite(output, len);
}