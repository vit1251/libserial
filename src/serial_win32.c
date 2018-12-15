#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include "serial.h"

struct serial_t {
    HANDLE _hPort;
    DCB *_dcb;
    COMMTIMEOUTS *_cto;
};

serial_t *serial_new(void)
{
    struct serial_t *s = NULL;
    DCB *dcb;
    COMMTIMEOUTS *cto;

    s = (struct serial_t *)malloc(sizeof(*s));
    if (s == NULL) {
        goto out;
    }
    // Create DCB
    dcb = (DCB *)malloc(sizeof(DCB));
    if (dcb == NULL) {
        free(s);
        s = NULL;
        goto out;
    }
    memset(dcb, 0, sizeof(DCB));
    dcb->DCBlength = sizeof(DCB);
    dcb->BaudRate = CBR_115200;
    dcb->fBinary = TRUE;
    dcb->fParity = FALSE;
    dcb->fOutxCtsFlow = FALSE;
    dcb->fDtrControl = DTR_CONTROL_DISABLE;
    dcb->fDsrSensitivity = FALSE;
    dcb->fTXContinueOnXoff = TRUE;
    dcb->fOutX = FALSE;
    dcb->fInX = FALSE;
    dcb->fErrorChar = FALSE;
    dcb->fNull = FALSE;
    dcb->fRtsControl = RTS_CONTROL_DISABLE;
    dcb->fAbortOnError = FALSE;
    dcb->wReserved = 0;
    dcb->XonLim = 4096;
    dcb->XoffLim = 4096;
    dcb->ByteSize = 8;
    dcb->Parity = NOPARITY;
    dcb->StopBits = ONESTOPBIT;
    dcb->XonChar = 0x11;
    dcb->XoffChar = 0x13;
    dcb->ErrorChar = 0x00;
    dcb->EofChar = 0x1A;
    dcb->EvtChar = 0x06;
    s->_dcb = dcb;

    // Create COMMTIMEOUTS
    cto = (COMMTIMEOUTS *)malloc(sizeof(COMMTIMEOUTS));
    if (cto==NULL) {
        free(s->_dcb);
        s->_dcb = NULL;
        free(s);
        s = NULL;
        goto out;
    }
    memset(cto, 0, sizeof(COMMTIMEOUTS));
    cto->ReadIntervalTimeout = 0xFFFFFFFF;
    cto->ReadTotalTimeoutMultiplier = 1;
    cto->ReadTotalTimeoutConstant = 1;
    cto->WriteTotalTimeoutMultiplier = 1;
    cto->WriteTotalTimeoutConstant = 1;
    s->_cto = cto;
  out:
    return s;
}

int serial_set_baud_rate(serial_t *serial, int boud_rate)
{
    int rc;
    if (serial==NULL) {
        rc = -1;
        goto out;
    }
    if (serial->_dcb==NULL) {
        rc = -2;
        goto out;
    }
    // TODO - validate on CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600, CBR_14400, CBR_19200, CBR_38400, CBR_56000, CBR_57600, CBR_115200, CBR_128000, CBR_256000
    serial->_dcb->BaudRate = boud_rate;
    // Complete
    rc = 0;
  out:
    return rc;
}

#define PORT_SIZE 64

int serial_open(serial_t *serial, short port)
{
    char port_name[PORT_SIZE];
    HANDLE hPort;
    int err;
    int rc;

    snprintf(port_name, PORT_SIZE, "\\\\.\\COM%d", port);

#ifndef _DEBUG
    fprintf(stdout, "debug: Prepare to open Win32 port %s\n", port_name);
#endif

    hPort = CreateFile(
        port_name,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0, // FILE_FLAG_OVERLAPPED
        NULL
    );
    if (hPort==INVALID_HANDLE_VALUE) {
        rc = -1;
        goto out;
    }
    err = SetCommState(hPort, serial->_dcb);
    if (err != 0) {
        rc = -2;
        goto out;
    }
    err = SetCommTimeouts(hPort, serial->_cto);
    if (err != 0) {
        rc = -3;
        goto out;
    }
    // Complete
    serial->_hPort = hPort;
    rc = 0;
  out:
    return rc;
}

int serial_write(serial_t *serial, unsigned char *buff, size_t size)
{
    DWORD feedback;
    int err;
    int rc;

    err = WriteFile(serial->_hPort, buff, (DWORD)size, &feedback, 0);
    if (err!=0) { 
        rc = -1;
        goto out;
    }
//    if (feedback != (DWORD)size) {
//        rc = -2;
//        goto out;
//    }
    rc = (int)feedback;
  out:
    return rc;
}

int serial_read(serial_t *serial, char *str, size_t size)
{
    DWORD dwSize;
    int err;
    int rc;
    //
    err = ReadFile(serial->_hPort, str, size, &dwSize, 0);
    if (err!=0) {
        rc = -1;
        goto out;
    }
    // Complete
    rc = (int)dwSize;
  out:
    return rc;
}

int serial_close(serial_t *serial)
{
    BOOL bResult;
    int rc;

    if (serial == NULL) {
        rc = -1;
        goto out;
    }
    bResult = CloseHandle(serial->_hPort);
    if (!bResult) {
        rc = -2;
        goto out;
    }
    // Complete
    serial->_hPort = NULL;
    rc = 0;
  out:
    return rc;
}

void serial_free(serial_t *serial)
{
    free(serial->_cto);
    free(serial->_dcb);
    free(serial);
}
