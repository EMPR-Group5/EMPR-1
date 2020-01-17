//#define const int addressLCD = 0x3b;

void lcdSetup(void);
void lcdWrite(char* data, int length);
void lcdClear(void);
void convertText(char* data, int length);
void lcdSetCursor(int pos);