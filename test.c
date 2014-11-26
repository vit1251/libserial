#include <stdio.h>
#include <string.h>

#include "serial.h"

#define BUFFER_SIZE 512

int main() {
    struct serial_t *serial = NULL;
    char *command = "\r\nATE0\r\n";
    int count = 10;
    char buffer[BUFFER_SIZE];
    size_t size;

    serial = serial_create();
    serial_open(serial, SERIAL_PORT_6);
 
    while (count > 0) {
        fprintf(stdout, "Iteration %d\n", count);

        fprintf(stdout, "Write modem command...\n");

        size = serial_write(serial, command, strlen(command));
        if (size > 0) {
            fprintf(stdout, "\t - write (size = %d)\n", size);
        }
        Sleep(100);
       
        fprintf(stdout, "Wait response from modem ...\n");
        size = (size_t)serial_read(serial, &buffer[0], BUFFER_SIZE);
        if (size > 0) {
            fprintf(stdout, "\t - read `%*s` bytes (size = %d)\n", size, buffer, size);
        }
        Sleep(200);

        count--;
    }
    serial_close(serial);
    serial_dispose(serial);

    return 0;
}