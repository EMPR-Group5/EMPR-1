# EMPR
![British Airways](https://goodlogo.com/images/logos/british_airways_logo_2591.gif)

###### main
* void main(void);

###### i2c 
* int i2cWrite(int addr, char* data, int length);
* int i2cRead(int addr, char* data, int length);
* int i2cReadWrite(int addr, char* writeData, int writeLength, char* readData, int readLength);
* void setupI2C(void);
* int i2cScan(int * addressArray);
* void i2cScanAll(void);

###### keypad 
* int keypadRead(void);
* int getNumber(int col, int readByte);
* void checkButton(int button, int * buffer, int * count, int bufferSize);

###### lcd 
* void lcdSetup(void);
* void lcdWrite(char* data, int length);
* void lcdClear(void);
* void lcdSetCursor(int pos);
* void lcdLineSelect(int line);
* void convertText(char* data, int length);

###### serial 
* int read_usb_serial_none_blocking(char *buf,int length);
* int write_usb_serial_blocking(char *buf,int length);
* char serialReadByte(void);
* void serialWriteByte(int byte);
* void stopSerial(void);
* void serial_init(void);

###### timer 
* void setupTimer(int interval);
* double timeElapsed(void);
* double totalTimeElapsed(void);
* void timerReset(void);
* void timerSleep(int duration);
* void SysTick_Handler(void);

###### utils 
* void initAll(void);
* void fillArray(int * array, int value, int length);
* void serialWrite(char *str);
* char * debug4Bit(int val);
