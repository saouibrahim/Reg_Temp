#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	float exterieure;
	float interieure;
} temp_t;

typedef struct
{
	int initFlag;	 // drapeau initialisation
	float erreur;	 // precedente erreur
	float integrale; // precedente integrale
} prec_t;

#define KP 1.1
#define KI 0.2
#define KD 0.15
#define dt 40.0

float err; // c'est l'erreur (n-1) dans la regulation

#endif