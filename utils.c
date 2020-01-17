#include "lpc17xx_gpio.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_uart.h"		
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_i2c.h"

char * debug4Bit(int val)
{
    GPIO_SetDir(1, 0xFFFFFFFF, 1);
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
    GPIO_SetValue(1, bits);
    return "0000";
}

int serialWrite(char *buf,int length)
{
	return(UART_Send((LPC_UART_TypeDef *)LPC_UART0,(uint8_t *)buf,length, BLOCKING));
}

void serialWriteByte(int byte)
{
    UART_SendByte((LPC_UART_TypeDef *)LPC_UART0, byte);
}

char serialRead(char *buf,int length)
{
	return(UART_Receive((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)buf, length, NONE_BLOCKING));
}

char serialReadByte(void)
{
    return(UART_ReceiveByte((LPC_UART_TypeDef *)LPC_UART0));
}

void setupSerial(void)
{
	UART_CFG_Type UARTConfigStruct;			// UART Configuration structure variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;	// UART FIFO configuration Struct variable
	PINSEL_CFG_Type PinCfg;				// Pin configuration for UART
	/*
	 * Initialize UART pin connect
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	// USB serial first
	PinCfg.Portnum = 0;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
		
	/* Initialize UART Configuration parameter structure to default state:
	 * - Baudrate = 9600bps
	 * - 8 data bit
	 * - 1 Stop bit
	 * - None parity
	 */


	UART_ConfigStructInit(&UARTConfigStruct);
	/* Initialize FIFOConfigStruct to default state:
	 * - FIFO_DMAMode = DISABLE
	 * - FIFO_Level = UART_FIFO_TRGLEV0
	 * - FIFO_ResetRxBuf = ENABLE
	 * - FIFO_ResetTxBuf = ENABLE
	 * - FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	// Built the basic structures, lets start the devices/
	// USB serial
	UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);		// Initialize UART0 peripheral with given to corresponding parameter
	UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);	// Initialize FIFO for UART0 peripheral
    
    //enable recieve interrupts
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RBR, ENABLE);
    // Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);
}

void stopSerial(void)
{
    UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, DISABLE);	
}


					
		
		
