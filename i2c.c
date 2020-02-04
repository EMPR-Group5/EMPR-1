#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "utils.h"

char output[128];
int len;

void setupI2C(void)
{
    //Configure pins for I2C1
    PINSEL_CFG_Type PinCfg;
    PinCfg.Funcnum = 3;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);

    //Init I2C1
    I2C_Init(LPC_I2C1, 100000);
    I2C_Cmd(LPC_I2C1, ENABLE);

    serialWrite("I2C setup");
}

int i2cWrite(int addr, char* data, int length)
{
    __disable_irq();
    //Setup Packet
    I2C_M_SETUP_Type packet;
    packet.sl_addr7bit = addr;
    packet.tx_data = data;
    packet.tx_length = length;
    packet.tx_count = 0;
    packet.rx_data = NULL;
    packet.rx_length = 0;
    packet.rx_count = 0;
    packet.retransmissions_max = 1;
    packet.retransmissions_count = 0;

    //Transfer Packet
    if(I2C_MasterTransferData(LPC_I2C1, &packet, (I2C_TRANSFER_OPT_Type) I2C_TRANSFER_POLLING) == SUCCESS)
    {   //if successful return 1
        __enable_irq();
        return(1);
    }
    else
    {   //Else return 0
        __enable_irq();
        return(0);
    }
}

int i2cRead(int addr, char* data, int length)
{    
    __disable_irq();
    //Setup Packet
    I2C_M_SETUP_Type packet;
    packet.sl_addr7bit = addr;
    packet.tx_data = NULL;
    packet.tx_length = 0;
    packet.tx_count = 0;
    packet.rx_data = data;
    packet.rx_length = length;
    packet.rx_count = 0;
    packet.retransmissions_max = 1;
    packet.retransmissions_count = 0;

    //Transfer Packet
    if(I2C_MasterTransferData(LPC_I2C1, &packet, (I2C_TRANSFER_OPT_Type) I2C_TRANSFER_POLLING) == SUCCESS)
    {   //if successful return 1
        __enable_irq();
        return(1);
    }
    else
    {   //Else return 0
        __enable_irq();
        return(0);
    }
}

int i2cReadWrite(int addr, char* writeData, int writeLength, char* readData, int readLength)
{    
    __disable_irq();
    //Setup Packet
    I2C_M_SETUP_Type packet;
    packet.sl_addr7bit = addr;
    packet.tx_data = writeData;
    packet.tx_length = writeLength;
    packet.tx_count = 0;
    packet.rx_data = readData;
    packet.rx_length = readLength;
    packet.rx_count = 0;
    packet.retransmissions_max = 1;
    packet.retransmissions_count = 0;

    //Transfer Packet
    if(I2C_MasterTransferData(LPC_I2C1, &packet, (I2C_TRANSFER_OPT_Type) I2C_TRANSFER_POLLING) == SUCCESS)
    {   //if successful return 1
        __enable_irq();
        return(1);
    }
    else
    {   //Else return 0
        __enable_irq();
        return(0);
    }
}

/*
int i2cScan(int* addressArray)
{
    int counter = 0;
    int testData [1] = {0x01};
    for(int addr = 0; addr < 128; addr++)
    {
        if(i2cWrite(addr, testData, 1) == 1)
        {
            addressArray[counter] = addr;
            counter ++;
        }
    }
    return(counter);
}

void i2cScanAll(void)
{
    //Scanning
    int* addresses[128];
    int num = i2cScan(addresses);
    len = sprintf(output, "%03d devices connected to i2c bus", num);
    serialWrite(output);

    for(int i = 0; i<num; i++)
    {
        len = sprintf(output, "Device at 0x%03x %03d", addresses[i], addresses[i]);
        serialWrite(output);
    }

    serialWrite("End of devices\n");
}
*/
