
const int SERIAL_BAUD_RATE = 9600;
const int SERIAL_BUFFER_MAX = 256;
const int SERIAL_READ_TIMEOUT = 1000; // msec
const int SERIAL_CONNECT_RETRY_INTERVAL = 3000; // msec
const char SERIAL_ALIVE_CODE[] = "7q8w9e";
const char* SERIAL_PORTS[20] = {
	"/dev/ttyACM0",
	"/dev/ttyACM1",
	"/dev/ttyACM2",
	"/dev/ttyACM3",
	"/dev/ttyACM4",
	"/dev/ttyACM5",
	"/dev/ttyACM6",
	"/dev/ttyACM7",
	"/dev/ttyACM8",
	"/dev/ttyACM9",
	"/dev/ttyACM10",
	"/dev/ttyACM11",
	"/dev/ttyACM12",
	"/dev/ttyACM13",
	"/dev/ttyACM14",
	"/dev/ttyACM15",
	"/dev/ttyACM16",
	"/dev/ttyACM17",
	"/dev/ttyACM18",
	"/dev/ttyACM19"
};

const int ALIVE_TIMEOUT = 4; // sec


#include "libraries/arduino-serial/arduino-serial-lib.h"
#include "libraries/arduino-serial/arduino-serial-lib.c"

int connect_to_serial();
int serial_port_is_valid(const char* serialPort);
void process_serial_readings(int fd, long int* lastTimeAlive);
void update_connection_state(int* fd, long int lastTimeAlive);
void find_open_serial_port(int openPorts[], int* openPortsCount);
void read_serial(int fd, char* buffer);
void execute_command(char* command);
