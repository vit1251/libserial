#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include "serial.h"





struct serial_t *serial_create() {
    struct serial_t *result;
    DCB *dcb;
    COMMTIMEOUTS *cto;

    result = (struct serial_t *)malloc(sizeof(struct serial_t));
    if (result != NULL) {
        // Create DCB
        dcb = (DCB *)malloc(sizeof(DCB));
        if (dcb != NULL) {
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

            result->_dcb = dcb;
        }
        // Create _cto
        cto = malloc(sizeof(COMMTIMEOUTS));
        if (cto != NULL) {
            memset(cto, 0, sizeof(COMMTIMEOUTS));
            cto->ReadIntervalTimeout = 0xFFFFFFFF;
            cto->ReadTotalTimeoutMultiplier = 1;
            cto->ReadTotalTimeoutConstant = 1;
            cto->WriteTotalTimeoutMultiplier = 1;
            cto->WriteTotalTimeoutConstant = 1;
            result->_cto = cto;
        }
    }
    return result;
}

void serial_dispose(struct serial_t *serial) {
    if (serial != NULL) {
        free(serial->_cto);
        free(serial->_dcb);
        free(serial);
    }
}

int serial_set_baud_rate(struct serial_t *serial, int boud_rate) {
    int result = -1;
    // CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400, CBR_4800, CBR_9600, CBR_14400, CBR_19200, CBR_38400, CBR_56000, CBR_57600, CBR_115200, CBR_128000, CBR_256000
    if (serial != NULL) {
        if (serial->_dcb != NULL) {
            serial->_dcb->BaudRate = boud_rate;
            result = 0;
        }
    }
    return result;
}

int serial_open(struct serial_t *serial, char *port) {
    int result = -1;
    HANDLE hPort;

    hPort = CreateFile(
        port,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0, // FILE_FLAG_OVERLAPPED
        NULL
    );
    if (hPort != INVALID_HANDLE_VALUE) {
        if(SetCommState(hPort, serial->_dcb)) {
            if (SetCommTimeouts(hPort, serial->_cto)) {
                serial->_hPort = hPort;
                result = 0;
            } else {
                result = -4;
            }
 	} else {
            result = -3;
        }
    } else {
        result = -2;
    }
    return result;
}

int serial_write(struct serial_t *serial, unsigned char *buff, size_t size) {
    int result = -1;
    DWORD feedback;

    if (WriteFile(serial->_hPort, buff, (DWORD)size, &feedback, 0)) { 
        if (feedback == (DWORD)size) {
            result = (int)feedback;
        } else {
            result = -3;
        }
    } else {
        result = -2;
    }

    return result;
}

int serial_read(struct serial_t *serial, char *str, size_t size) {
    int result = -1;

    DWORD dwSize;

    if(ReadFile(serial->_hPort, str, size, &dwSize, 0)) {
        result = (int)dwSize;
    } else {
        result = -2;
    }

    return result;
}

int serial_close(struct serial_t *serial) {
    int result = -1;
    BOOL bResult;

    if (serial != NULL) {
        bResult = CloseHandle(serial->_hPort);
        if (bResult) {
            serial->_hPort = NULL;
            result = 0;
        } else {
            result = -2;
        }
    }

    return result;
}

