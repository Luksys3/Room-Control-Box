int serial_init(const char* serialport, int baud);
int serial_writebyte(int fd, uint8_t b);
int serial_write(int fd, const char* str);
int serial_read_until(int fd, char* buf, char until);
