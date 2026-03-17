#ifndef REGULATION_H       
#define REGULATION_H

#include <stdio.h>         
#include <stdlib.h>        
#include "define.h"        // Accès à KP, KI, KD, dt, temp_t

float regulationTest(int regul, float csgn, float* tabT, int nT);  

#endif