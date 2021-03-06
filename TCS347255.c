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

    serialWrite("TCSRGB setup");
}

int TCSRead(int* RGBC)
{
    // Read 8 bytes of data from register(0x94)
	// cData lsb, cData msb, red lsb, red msb, green lsb, green msb, blue lsb, blue msb
	
    char data[8] = {0};
    i2cReadWrite(TCSADDRESS, CDATAREAD, 1, data, 8);

    // Convert the data
    // RGBC[3]   = (data[1] << 8) + data[0];
    // RGBC[0]   = (data[3] << 8) + data[2];
    // RGBC[1]   = (data[5] << 8) + data[4];
    // RGBC[2]   = (data[7] << 8) + data[6];
	RGBC[3]   = data[1];
    RGBC[0]   = data[3];
    RGBC[1]   = data[5];
    RGBC[2]   = data[7];
  
    char output[128];
    // sprintf(output, "Current RGBC value: %d, %d, %d", RGBC[0], RGBC[1], RGBC[2]);
    serialWrite(output);
}

void TCSReadConverted(int* RGBC)
{
	int r = RGBC[0];
    int g = RGBC[1];
    int b = RGBC[2];

    r = (r % 3) > 0  ? (++r % 3) > 0  ? ++r : r : r;
    g = (g % 3) > 0  ? (++g % 3) > 0  ? ++g : g : g;
    b = (b % 3) > 0  ? (++b % 3) > 0  ? ++b : b : b;

	char output[128];
    sprintf(output, "[%d, %d, %d] => [%d, %d, %d]", RGBC[0], RGBC[1], RGBC[2], r, g, b);
    serialWrite(output);

	RGBC[0] = r;
	RGBC[1] = g;
	RGBC[2] = b;
}