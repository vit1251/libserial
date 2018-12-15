#ifndef _SERIAL_H
#define _SERIAL_H

typedef struct serial_t serial_t;

serial_t *serial_new();
int serial_set_baud_rate(serial_t *serial, int boud_rate);
int serial_open(serial_t *serial, short port);
int serial_write(serial_t *serial, unsigned char *buff, size_t size);
int serial_read(serial_t *serial, char *str, size_t size);
int serial_close(serial_t *serial);
void serial_free(serial_t *serial);

#endif /* _SERIAL_H */
