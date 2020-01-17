#include "lpc17xx_i2c.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "utils.h"

int i2cWrite(int addr, char* data, int length)
{

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
        return(1);
    }
    else
    {   //Else return 0
        return(0);
    }
}

int i2cRead(int addr, char* data, int length)
{    
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
        return(1);
    }
    else
    {   //Else return 0
        return(0);
    }
}

int i2cReadWrite(int addr, char* writeData, int writeLength, char* readData, int readLength)
{    
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
        return(1);
    }
    else
    {   //Else return 0
        return(0);
    }
}

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
    I2C_Init(LPC_I2C1, 10000);
    I2C_Cmd(LPC_I2C1, ENABLE);
}

int i2cScan(int * addressArray)
{
    int counter = 0;
    int addr = 0;
    int testData [1] = {0x01};
    for(addr = 0; addr < 128; addr++)
    {
        if(i2cWrite(addr, testData, 1) == 1)
        {
            addressArray[counter] = addr;
            counter ++;
        }
    }
    return(counter);
}