#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	float exterieure;
	float interieure;
} temp_t;

#define KP 1.1
#define KI 0.2
#define KD 0.15
#define dt 10.0

#endif