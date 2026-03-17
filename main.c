#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     

#include "define.h"
#include "consigne.h"
#include "regulation.h"
#include "visualisationT.h"
#include "visualisationC.h"
#include "simulateur.h"

int main() {
    temp_t maTemp = {15.0, 10.0};
    float maConsigne = 19.0;
    float commande = 0.0;
    int regul = 2;

    int nT = 0;
    float tabT[10000];

    struct simParam_s* monSimulateur = simConstruct(maTemp);

    printf("Démarrage de la simulation...\n");

    while (1) {
        maConsigne = consigne(maConsigne);

        if (maConsigne <= 5.0) {
            commande = 0.0;
            visualisationC(commande);
            printf("Consigne à 5°C : Arrêt du chauffage et fin du programme.\n");
            break;
        }

        tabT[nT] = maTemp.interieure;
        nT++;

        commande = regulationTest(regul, maConsigne, tabT, nT);
        maTemp = simCalc(commande, monSimulateur);

        visualisationT(maTemp);
        visualisationC(commande);

        sleep(1); 
    }

    simDestruct(monSimulateur);
    return 0;
}