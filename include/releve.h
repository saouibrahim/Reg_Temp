#ifndef RELEVE_H
#define RELEVE_H

#include <windows.h>
#include <stdio.h>
#include "define.h"
#include "ftd2xx.h"

void releve(temp_t *temperature, FT_HANDLE ftHandle);

#endif