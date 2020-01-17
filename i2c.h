//#define bool i2cIsSetup = false;
int i2cWrite(int addr, char* data, int length);
int i2cRead(int addr, char* data, int length);
int i2cReadWrite(int addr, char* writeData, int writeLength, char* readData, int readLength);
void setupI2C(void);
int i2cScan(int * addressArray);
void i2cScanAll(void);