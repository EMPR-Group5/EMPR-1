#include "utils.h"
#include "i2c.h"
#include "timer.h"


double lookupTimes[5][5] = {{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};


int keypadRead(void)
{
    int f = -1;
    int c;
    for(c = 0; c < 4; c++)
    {
        char readByte[1];
        char writeByte[] = {~(1<<(4+c)) | 0x0f};
        int x = i2cReadWrite(0x21, writeByte, 1, readByte, 1);        
        readByte[0] = (~readByte[0]) & 0x0f;
        if(readByte[0] > 0)
        {       
            f = getNumber(c, readByte[0]);     
        }
    }
    return (f);
}

int getNumber(int col, int readByte)
{

    double debounceTime = 0.5;

    col = 3-col;
    int row = 0;
    char lookup[5][5] = {{' ','1','4','7','*'},{' ','2','5','8','0'},{' ','3','6','9','#'},{' ','A','B','C','D'}};
    
    char button;

    int z;
    for(z=0; z<4;z++)
    {
        int rb = readByte;
         if((rb & (1<<z)) == (1<<z))
         {
            row = z;
         }
    }
    row = 4-row;

    double currentTime = totalTimeElapsed();
    double lastPressed = lookupTimes[col][row];
    double timeSince = currentTime - lastPressed;
    
    if(timeSince > debounceTime)
    {   
        lookupTimes[col][row] = currentTime;
        button = lookup[col][row];
        return ((int)button);
    }
    else
    {
        return (-1);
    }  
}