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
    temp_t maTemp = {15.0, 10.0}; // temp int et ext
    float maConsigne = 19.0; // temp voulue
    float commande = 0.0; // puiss de chauffage
    int regul = 2; // ToutouRien ou PID
    int nT = 0; // nb de temp enregistrées
    float tabT[10000]; // tab des temps

    // création et init du simu 
    struct simParam_s* monSimulateur = simConstruct(maTemp);

    printf("Démarrage de la simulation...\n");

    while (1) {
        // lecture du fichier consigne.txt avec l'appel de 'consigne.h'
        maConsigne = consigne(maConsigne);

        // si la temp est inf à 5deg, on break 
        if (maConsigne <= 5.0) {
            commande = 0.0;
            visualisationC(commande);
            printf("Consigne à 5°C, le chauffage s'arrête. Fin de programme.\n");
            break;
        }

        // enregistre la temp int actuelle dans le tableau
        if (nT < 10000) {
            tabT[nT] = maTemp.interieure;
            nT++;
        }

        // calcul de la puis de chauffe via regul == (1 ou 2)
        commande = regulationTest(regul, maConsigne, tabT, nT);
        // maj de la temp via "simulateur.h"
        maTemp = simCalc(commande, monSimulateur);
        // affiche temp int et ext
        visualisationT(maTemp);
        // affiche puis de chauffe 
        visualisationC(commande);

        sleep(1);
    }

    simDestruct(monSimulateur);
    return 0;
}