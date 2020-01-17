
char * debug4Bit(int val);
void setupSerial(void);
int serialWrite(char *buf,int length);
void serialWriteByte(int byte);
char serialRead(char *buf, int length);
char serialReadByte(void);
void stopSerial(void);
//bool i2cSendComplete(void);
