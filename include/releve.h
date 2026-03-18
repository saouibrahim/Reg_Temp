#ifndef RELEVE_H
#define RELEVE_H

#include <windows.h>
#include <stdio.h>
#include "define.h"
#include "ftd2xx.h"

DWORD RxBytes;
DWORD TxBytes;
DWORD EventDWord;
DWORD BytesReceived;
char RxBuffer[6]; // Buffer for exactly 6 bytes, where 3 are for the Tint and 3 for the Text

#endif