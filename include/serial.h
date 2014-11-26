#ifndef _SERIAL_H
#define _SERIAL_H

#ifdef _WIN32
#include <windows.h>
#endif

#define SERIAL_PORT_1 "\\\\.\\COM1"
#define SERIAL_PORT_2 "\\\\.\\COM2"
#define SERIAL_PORT_3 "\\\\.\\COM3"
#define SERIAL_PORT_4 "\\\\.\\COM4"
#define SERIAL_PORT_5 "\\\\.\\COM5"
#define SERIAL_PORT_6 "\\\\.\\COM6"
#define SERIAL_PORT_7 "\\\\.\\COM7"
#define SERIAL_PORT_8 "\\\\.\\COM8"
#define SERIAL_PORT_9 "\\\\.\\COM9"
#define SERIAL_PORT_10 "\\\\.\\COM10"
#define SERIAL_PORT_11 "\\\\.\\COM11"

struct serial_t {
    HANDLE _hPort;
    DCB *_dcb;
    COMMTIMEOUTS *_cto;
};

struct serial_t *serial_create();
void serial_dispose(struct serial_t *serial);
int serial_set_baud_rate(struct serial_t *serial, int boud_rate);
int serial_open(struct serial_t *serial, char *port);
int serial_write(struct serial_t *serial, unsigned char *buff, size_t size);
int serial_read(struct serial_t *serial, char *str, size_t size);
int serial_close(struct serial_t *serial);

#endif /* _SERIAL_H */
