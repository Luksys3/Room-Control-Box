#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "header.h"

int main() {
	long int lastTimeAlive;
	int fd = -1;

	while (1) {
		// Connection
		int newConnectionMade = 0;
		if (fd == -1) {
			fd = connect_to_serial();
			newConnectionMade = (fd != -1) ? 1 : 0;

			if (newConnectionMade == 0) {
				printf("Found 0 valid serial ports.\n\n");
				usleep(SERIAL_CONNECT_RETRY_INTERVAL * 1000);
				continue;
			}
		}
		if (newConnectionMade == 1)
			lastTimeAlive = time(0);


		// Read serial
		process_serial_readings(fd, &lastTimeAlive);

		// Check if alive
		update_connection_state(&fd, lastTimeAlive);
	}

	return 0;
}

void process_serial_readings(int fd, long int* lastTimeAlive) {
	char buffer[SERIAL_BUFFER_MAX];
	read_serial(fd, buffer);
	if (strcmp(buffer, "") == 0)
		return;

	// Check alive code
	if (strncmp(buffer, SERIAL_ALIVE_CODE, sizeof(SERIAL_ALIVE_CODE) - 1) == 0) {
		*lastTimeAlive = time(0);
		return;
	}

	printf("Command %ld: %s", time(0), buffer);
	execute_command(buffer, fd);
}

void update_connection_state(int* fd, long int lastTimeAlive) {
	if (time(0) - lastTimeAlive <= ALIVE_TIMEOUT)
		return;

	serialport_close(*fd);
	*fd = -1;
	printf("DISCONNECTED. Searching for serial port.\n");
}

int connect_to_serial() {
	int openPorts[sizeof(SERIAL_PORTS) / sizeof(SERIAL_PORTS[0])], openPortsCount;
	find_open_serial_port(openPorts, &openPortsCount);

	for (int i = 0; i < openPortsCount; i++) {

		printf("Validating port %s...", SERIAL_PORTS[openPorts[i]]);
		if (serial_port_is_valid(SERIAL_PORTS[openPorts[i]]) == 1) {
			printf(" valid.\n");
			return serialport_init(SERIAL_PORTS[openPorts[i]], SERIAL_BAUD_RATE);
		} else
			printf(" invalid.\n");

	}

	return -1;
}

void find_open_serial_port(int openPorts[], int* openPortsCount) {
	int fd = -1;
	*openPortsCount = 0;

	for (int portIndex = 0; portIndex < sizeof(SERIAL_PORTS) / sizeof(SERIAL_PORTS[0]); portIndex++) {
		fd = serialport_init(SERIAL_PORTS[portIndex], SERIAL_BAUD_RATE);

		if (fd != -1) {
			serialport_close(fd);
			printf("Found open port: %s\n", SERIAL_PORTS[portIndex]);
			openPorts[*openPortsCount] = portIndex;
			*openPortsCount += 1;
		}
	}
}

int serial_port_is_valid(const char* serialPort) {
	char buffer[SERIAL_BUFFER_MAX], valid = 0;
	long int started = time(0);
	int fd = serialport_init(serialPort, SERIAL_BAUD_RATE);

	while (time(0) - started < ALIVE_TIMEOUT) {
		read_serial(fd, buffer);
		if (strncmp(buffer, SERIAL_ALIVE_CODE, sizeof(SERIAL_ALIVE_CODE) - 1) == 0) {
			valid = 1;
			break;
		}
	}

	serialport_close(fd);
	return valid;
}

void read_serial(int fd, char* buffer) {
	memset(buffer, 0, sizeof buffer);
	serialport_read_until(fd, buffer, '\n', SERIAL_BUFFER_MAX - 1, SERIAL_READ_TIMEOUT);
}

void execute_command(char command[], int fd) {
	if (strncmp(command, "launch_chrome", sizeof "launch_chrome" - 1) == 0)
		system("google-chrome \"http://youtube.com/results?search_query=c+lang\"");
	else if (strncmp(command, "BUTTON PRESSED", sizeof "BUTTON PRESSED" - 1) == 0) {
		printf("COMMAND SENT\n");
		serialport_write(fd, "testdd");
	}
}
