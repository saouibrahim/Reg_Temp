#ifndef VISUALISATIONT_H
#define VISUALISATIONT_H

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "define.h"

void visualisationT(temp_t maTemperature); // ont voulais mettre maTemperature en pointeur pour éviter de faire une copie de la structure temp_t qui contient 2 float
// mais pour faire marcher les tests unitaires sur moodle il faut passer sans pointeur
#endif
