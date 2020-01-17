int read_usb_serial_none_blocking(char *buf,int length);
int write_usb_serial_blocking(char *buf,int length);
char serialReadByte(void);
void serialWriteByte(int byte);
void stopSerial(void);
void serial_init(void);