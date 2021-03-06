#include <stdio.h>  // Global includes
#include "utils.h"  // Local functions
#include "i2c.h"

const int LCDADDRESS = 0x3b;

void lcdSetup(void)
{
    char initial[] = {0x00,0x34,0x0c,0x06,0x35,0x04,0x10,0x42,0x9f,0x34,0x02};
    i2cWrite(LCDADDRESS, initial, 11);
    lcdClear();
    serialWrite("LCD setup");
}

void lcdWrite(char* data, int length)
{
    //Insert command byte at start
    convertText(data, length);
    
    char text[length+1];
    text[0] = 0x40;
    int a;
    for(a=0; a<length; a++)
    {
        text[1+a] = data[a];
    }
    //Send new data array
    i2cWrite(LCDADDRESS, text, length+1);
}

void lcdClear(void)
{   
    int len = 81;
    char clear [len];
    memset(clear, 0xA0, len);
    clear[0] = 0x40;
    lcdSetCursor(0);
    i2cWrite(LCDADDRESS, clear, len);
    lcdSetCursor(0);
}

void lcdSetCursor(int pos)
{
    char data [2] = {0x00, 0x00};
    data[1] = 0x80+pos;
    i2cWrite(LCDADDRESS, data, 2);
}

void lcdLineSelect(int line)
{
    int cursor = (line == 1 ? 0 : 41);
    lcdSetCursor(cursor);
}

void convertText(char* data, int length)
{
    int a;
    for(a = 0; a<length; a++)
    {
        int val = data[a];
        if((val >= 0x20) && (val <= 0x5A))
        {
            data[a] = val+128;
        }
    }
}