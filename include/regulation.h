#ifndef REGULATION_H       
#define REGULATION_H

#include <stdio.h>         
#include <stdlib.h> 
#include <stdbool.h>
#include "define.h"        // Accès à KP, KI, KD, dt, temp_t

float regulationTest(int regul, float csgn, float* tabT, int nT); 
float regulation_1ou0(float csgn, float temperature_int);
float regulation_PID(float csgn, float temperature_int, float *erreur_prec, float *int_prec, bool *isFirst);



#endif