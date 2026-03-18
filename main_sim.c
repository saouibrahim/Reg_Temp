#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "define.h"
#include "consigne.h"
#include "regulation.h"
#include "visualisationT.h"
#include "visualisationC.h"
#include "simulateur.h"

int main()
{
    temp_t maTemp = {15.0, 10.0}; // temp int et ext
    float maConsigne = 19.0;      // temp voulue
    float commande = 0.0;         // puiss de chauffage
    int regul = 2;                // ToutouRien ou PID
    int nT = 0;                   // nb de temp enregistrées
    float tabT[10000];            // tab des temps

    struct simParam_s *monSimulateur = simConstruct(maTemp);

    printf("Démarrage de la simulation...\n");

    while (1)
    {
        maConsigne = consigne(maConsigne);

        // si la temp est inf à 5deg
        if (maConsigne <= 5.0)
        {
            commande = 0.0;
            visualisationC(commande);
            printf("Consigne à 5°C, le chauffage s'arrête. Fin de programme.\n");
            break;
        }

        if (nT < 10000)
        {
            tabT[nT] = maTemp.interieure;
            nT++;
        }

        commande = regulationTest(regul, maConsigne, tabT, nT);
        maTemp = simCalc(commande, monSimulateur);

        visualisationT(maTemp);
        visualisationC(commande);

        sleep(1);
    }

    simDestruct(monSimulateur);
    return 0;
}