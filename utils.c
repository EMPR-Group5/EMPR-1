#include <stdio.h>          // Global includes
#include <stdlib.h>
#include "lpc17xx_gpio.h"   // Central include files // Local functions
#include "serial.h"         // Local functions

int messageCount = 0;

void initAll(void)
{
    serial_init();
    setupI2C();
    setupTimer();
    lcdSetup();
    serialWrite("\rStarting\n\r");
}

void fillArray(int * array, int value, int length)
{
    int a = 0;
    for(a=0; a<length;a++)
    {
        array[a] = value;
    }
}

void serialWrite(char *str)
{
    // malloc for the output string to save space
    char *buffer = (char*) malloc((strlen(str) + sizeof(int) + 3) * sizeof(char));
    sprintf(buffer, "<%d> %s\n\r", messageCount, str);
    write_usb_serial_blocking(buffer, strlen(buffer));

    free(buffer);
    messageCount++;
}

char * debug4Bit(int val)
{
    int LEDS[4] = {0x40000, 0x100000, 0x200000, 0x800000};
    int bits = 0;
    GPIO_ClearValue(1, LEDS[0] | LEDS[1] | LEDS[2] | LEDS[3]);
    switch(val)
    {
        case 0:
            bits = 0x0;
            GPIO_SetValue(1, bits);
            return "0000";
        case 1: 
            bits = LEDS[0];
            GPIO_SetValue(1, bits);
            return "0001";
        case 2:
            bits = LEDS[1];
            GPIO_SetValue(1, bits);
            return "0010";
        case 3:
            bits = LEDS[0] | LEDS[1];
            GPIO_SetValue(1, bits);
            return "0011";
        case 4:
            bits = LEDS[2];
            GPIO_SetValue(1, bits);
            return "0100";
        case 5:
            bits = LEDS[0] | LEDS[2];
            GPIO_SetValue(1, bits);
            return "0101";
        case 6:
            bits = LEDS[1] | LEDS[2];
            GPIO_SetValue(1, bits);
            return  "0110";
        case 7:
            bits = LEDS[0] | LEDS[1] | LEDS[2];
            GPIO_SetValue(1, bits);
            return "0111";
        case 8:
            bits = LEDS[3];
            GPIO_SetValue(1, bits);
            return "1000";
        case 9:
            bits = LEDS[0] | LEDS[3];
            GPIO_SetValue(1, bits);
            return "1001";
        case 10:
            bits = LEDS[1] | LEDS[3];
            GPIO_SetValue(1, bits);
            return "1010";
        case 11:
            bits = LEDS[0] | LEDS[1] | LEDS[3];
            GPIO_SetValue(1, bits);
            return "1011";
        case 12:
            bits = LEDS[2] | LEDS[3];
            GPIO_SetValue(1, bits);
            return "1100";
        case 13:
            bits = LEDS[0] | LEDS[2] | LEDS[3];
            GPIO_SetValue(1, bits);
            return "1101";
        case 14:
            bits = LEDS[1] | LEDS[2] | LEDS[3];
            GPIO_SetValue(1, bits);
            return "1110";
        case 15:
            bits = LEDS[0] | LEDS[1] | LEDS[2] | LEDS[3];
            GPIO_SetValue(1, bits);
            return "1111";
        default:
            bits = LEDS[0] | LEDS[1] | LEDS[2] | LEDS[3];
            GPIO_SetValue(1, bits);
            return "0000";
    }
}
