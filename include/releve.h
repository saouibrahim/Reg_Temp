#ifndef RELEVE_H
#define RELEVE_H

#include <windows.h>
#include <stdio.h>
#include "define.h"
#include "ftd2xx.h"

extern DWORD RxBytes;
extern DWORD TxBytes;
extern DWORD EventDWord;
extern DWORD BytesReceived;
extern char RxBuffer[6]; // Buffer for exactly 6 bytes, where 3 are for the Tint and 3 for the Text

void releve(temp_t *temperature, FT_HANDLE ftHandle);

#endif