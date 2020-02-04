#include "motorController.h"
#include "utils.h"
#include <math.h>
#include "TCS347255.h"

int scanType = 0;
int current[3] = {0,0,0};
int resolution;
char* imageArray;
int finished = 1;


void StartScan(int scanTipe, int res, char* imageArr)
{
    scanType = scanTipe;
    resolution = res;
    imageArray = imageArr;
    current[0] = 0;
    current[1] = 0;
    current[2] = 0;
    finished = 0;
}


void UpdateScan()
{
    if(finished == 0)
    {
        if(TargetReached() == 1)
        {
            switch (scanType)
            {
                case 0:
                    UpdateRasterScan();
                    break;
                case 1:
                    UpdateCircleScan();
                    break;
                case 2:
                    UpdateRectScan();
                    break;
            }
        }
    }
}



void UpdateRasterScan()
{
    int* data[4];
    TCSRead(data);
    char output[64];
    sprintf(output, "Values at %d %d:", current[0], current[1]);
    serialWrite(output);
    sprintf(output, "    %d, %d, %d, %d", data[0], data[1], data[2], data[3]);
    serialWrite(output);


    int stepSize = 700/resolution;
    int offset = (int) stepSize/2;


    if((current[0] >= resolution-1) & (current[1] >= resolution-1))
    {
        finished = 1;
    }
    else if(current[1] < resolution-1)
    {
        current[1]++;
    }
    else if(current[1] >= resolution-1)
    {
        current[1] = 0;
        if(current[0] < resolution-1)
        {
            current[0]++;
        }
    }



    int targetY = (int)((current[1]*stepSize) + offset);
    SetSingleCoordinate(1, targetY);
    int targetX = (int)((current[0]*stepSize) + offset);
    SetSingleCoordinate(0, targetX);


}

void UpdateRectScan(int resolution)
{
    serialWrite("Rect\n\r");
    int stepSize = 700/resolution;
    int offset = (int) stepSize/2;
    int x = current[0];
    int y = current[1];
    for(x = 0; x<resolution; x ++);
    {
        SetSingleCoordinate(0, (int)((x*stepSize) + offset));
        SetSingleCoordinate(1, (int)((y*stepSize) + offset));
    }
    for(y = 0; y<resolution; y ++)
    {
        SetSingleCoordinate(0, (int)((x*stepSize) + offset));
        SetSingleCoordinate(1, (int)((y*stepSize) + offset));
    }
    for(x = x; x>0; x --)
    {
        SetSingleCoordinate(0, (int)((x*stepSize) + offset));
        SetSingleCoordinate(1, (int)((y*stepSize) + offset));
    }
    for(y = y; y>0; y --)
    {
        SetSingleCoordinate(0, (int)((x*stepSize) + offset));
        SetSingleCoordinate(1, (int)((y*stepSize) + offset));
    }
}

void UpdateCircleScan()
{
    serialWrite("Circ\n\r");
    float stepSize = (2*3.14)/resolution;
    if(current[0] < resolution - 1)
    {
        current[0]++;
        int x = 350 + (200 * cos(current[0]*stepSize));
        int y = 350 + (200 * sin(current[0]*stepSize));  
        SetSingleCoordinate(0, x);
        SetSingleCoordinate(1, y);
    }
    else
    {
        finished = 1;
    }
}