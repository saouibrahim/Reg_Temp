#include <stdio.h>
#include <stdlib.h>

#include<windows.h>

#include "simulateur.h"
/**
#include "consigne.h"
#include "regulation.h"
#include "visualisationC.h"
#include "visualisationT.h"
**/

// Voir le fichier trace.txt pour comprendre la baisse de température

int main(){

	temp_t temperature; // initialisation de la température
	temperature.exterieure = 10.0; // initialisation de la température extérieure à 14.0°C
	temperature.interieure = 15.0; // initialisation de la température intérieure à 15.0°C

    float cmd = 0; // initialisation de la commande du chauffage à 0%
	struct simParam_s*  monSimulateur = simConstruct(temperature); // creation du simulateur, puissance intialisé
    // programme qui s'arrete quand la température intérieure s'approche de la température extérieure
	while(temperature.interieure - temperature.exterieure >= 0.5 ){
        temperature = simCalc(cmd,monSimulateur); // calcule de la température grâce à la commande en % du chauffage via simCalc
		printf("int %f, ext %f\n", temperature.exterieure, temperature.interieure);
		usleep(40000);
	}
	
	printf("Il ne reste plus qu'a allumer le chauffage !\n");
	
	simDestruct(monSimulateur); // destruction de simulateur
	return EXIT_SUCCESS;
}