
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "serial.h"

#define BUFFER_SIZE 512

int main()
{
    struct serial_t *serial = NULL;
    const unsigned char *command = "\r\nATE0\r\n";
    int count = 10;
    char *mem = NULL;
    size_t size;
    const int port = 6;
    int err;
    int rc;

    // Create memory
    mem = malloc(1024);
    if (mem == NULL) {
        fprintf(stderr, "Unable to allocate memory.\n");
        rc = -1;
        goto out;
    }

    serial = serial_new();
    if (serial == NULL) {
        fprintf(stderr, "Unable to create new serial instance.\n");
        rc = 1;
        goto out;
    }

    err = serial_open(serial, port);
    if (err != 0) {
        fprintf(stderr, "Unable to open serial port %d.\n", port);
        rc = 1;
        goto out;
    }
 
    while (count > 0) {
        fprintf(stdout, "Iteration %d\n", count);

        fprintf(stdout, "Write modem command...\n");

        size = serial_write(serial, command, strlen(command));
        if (size > 0) {
            fprintf(stdout, "\t - write (size = %zu)\n", size);
        }
        usleep(100 * 1000);
       
        fprintf(stdout, "Wait response from modem ...\n");
        size = (size_t)serial_read(serial, mem, BUFFER_SIZE);
        if (size > 0) {
            fprintf(stdout, "\t - read `%*s` bytes (size = %zu)\n", (int)size, mem, size);
        }
        usleep(200 * 1000);

        count--;
    }

    rc = 0;

  out:

    if (serial != NULL) {
        serial_close(serial);
        serial_free(serial);
        serial = NULL;
    }

    if (mem != NULL) {
        free(mem);
        mem = NULL;
    }

    return rc;
}
