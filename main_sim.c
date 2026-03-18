#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "include/define.h"
#include "include/consigne.h"
#include "include/regulation.h"
#include "include/visualisationT.h"
#include "include/visualisationC.h"
#include "include/simulateur.h"

int main()
{
    temp_t maTemp = {.interieure = 15.0, .exterieure = 10.0};
    float maConsigne = 19.0;
    float commande = 0.0;
    int regul = 2; // 1 = tout ou rien, 2 = PID

    // PID persistent state
    float erreur_prec = 0.0;
    float int_prec = 0.0;
    bool isFirst = true;

    struct simParam_s *monSimulateur = simConstruct(maTemp);

    printf("Démarrage de la simulation...\n");

    while (1)
    {
        // Update setpoint from file if changed
        maConsigne = consigne(maConsigne);

        // Safety stop if setpoint drops to or below 5°C
        if (maConsigne <= 5.0)
        {
            commande = 0.0;
            visualisationC(commande);
            printf("Consigne à %.1f°C (<= 5°C), arrêt du chauffage. Fin de programme.\n", maConsigne);
            break;
        }

        // Compute command depending on chosen regulation mode
        if (regul == 1)
        {
            commande = regulation_1ou0(maConsigne, maTemp.interieure);
        }
        else if (regul == 2)
        {
            commande = regulation_PID(maConsigne, maTemp.interieure, &erreur_prec, &int_prec, &isFirst);
            isFirst = false;
        }
        else
        {
            printf("Mode de régulation invalide (%d). Utiliser 1 (tout-ou-rien) ou 2 (PID).\n", regul);
            commande = 0.0;
        }

        // Simulate the thermal system (replaces STM32 hardware)
        maTemp = simCalc(commande, monSimulateur);

        // Update visualisation files
        visualisationT(&maTemp);
        visualisationC(commande);

        printf("Consigne: %.2f°C | Int: %.2f°C | Ext: %.2f°C | Cmd: %.1f%%\n",
               maConsigne, maTemp.interieure, maTemp.exterieure, commande);

        sleep(1);
    }

    simDestruct(monSimulateur);
    return EXIT_SUCCESS;
}