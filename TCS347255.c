#include <stdio.h>              // Global includes
#include "utils.h"			    // Local functions
#include "i2c.h"
#include "timer.h"

const int TCSADDRESS = 0x29;

// I2C commands to read certain colours
const char CDATAREAD[1] = {0x94};
const char CDATAREADH[1] = {0x95};
const char REDREAD[1] = {0x96};
const char REDREADH[1] = {0x97};
const char GREENREAD[1] = {0x98};
const char GREENREADH[1] = {0x99};
const char BLUEREAD[1] = {0x9A};
const char BLUEREADH[1] = {0x9B};

void setupTCS()
{
    // Select enable register(0x80)
	// Power ON, RGBC enable, wait time disable(0x03)
	char config[2] = {0};
	config[0] = 0x80;
	config[1] = 0x03;
	i2cWrite(TCSADDRESS, config, 2);

	// Select ALS time register(0x81)
	// Atime = 700 ms(0x00)
	config[0] = 0x81;
	config[1] = 0x00;
	i2cWrite(TCSADDRESS, config, 2);

	// Select Wait Time register(0x83)
	// WTIME : 2.4ms(0xFF)
	config[0] = 0x83;
	config[1] = 0xFF;
	i2cWrite(TCSADDRESS, config, 2);

	// Select control register(0x8F)
	// AGAIN = 1x(0x00)
	config[0] = 0x8F;
	config[1] = 0x00;
	i2cWrite(TCSADDRESS, config, 2);

	timerSleep(1);

    serialWrite("TCSRGB setup");
}

int TCSRead()
{
    // Read 8 bytes of data from register(0x94)
	// cData lsb, cData msb, red lsb, red msb, green lsb, green msb, blue lsb, blue msb
    
	// i2cWrite(TCSADDRESS, reg, 1);
	int data[6];
    i2cReadWrite(TCSADDRESS, &REDREAD, 1, &data, 6);

    // Convert the data
    // char cData = ((data[1] << 8) + data[0]);
    int red     = (data[1] << 8) + data[0];
    int green   = (data[3] << 8) + data[2];
    int blue    = (data[5] << 8) + data[4];

    // char data[2];
    // char output[128];

    // // Convert the data
    // int code
    // code = i2cReadWrite(TCSADDRESS, CDATAREAD, 1, data, 2);
    // int cData = (int) data[1];

    // serialWrite(code);

    // sprintf(output, "Current CDATA value: %d", cData);
    // serialWrite(output);
    
    // memset(output, 0, 128);

    // code = i2cReadWrite(TCSADDRESS, REDREAD, 1, data, 2);
    // serialWrite(code);
    // int red = (int) data[1];
    // sprintf(output, "Current RED value: %d", red);
    // serialWrite(output);
    
    // memset(output, 0, 128);

    // code = i2cReadWrite(TCSADDRESS, GREENREAD, 1, data, 2);
    // serialWrite(code);
    // int green = (int) data[1];
    // sprintf(output, "Current GREEN value: %d", green);
    // serialWrite(output);
    
    // memset(output, 0, 128);

    // code = i2cReadWrite(TCSADDRESS, BLUEREAD, 1, data, 2);
    // serialWrite(code);
    // int blue = (int) data[1];
    // sprintf(output, "Current BLUE value: %d", blue);
    // serialWrite(output);
    
    // memset(output, 0, 128);

    // int cData = (data[0]);
    // int red = (data[2]);
    // int green = (data[4]);
    // int blue = (data[6]);
    
    serialWrite("Finished UwU\n");
    char output[128];
    red = 10;
    green = 10;
    blue = 10;
    sprintf(output, "Current RGBC value: %d, %d, %d", red, green, blue);

    serialWrite(red);
    serialWrite(green);
    serialWrite(blue);
    //serialWrite(&output);
}