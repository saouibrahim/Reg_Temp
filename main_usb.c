#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "include/define.h"
#include "include/consigne.h"
#include "include/regulation.h"
#include "include/visualisationT.h"
#include "include/visualisationC.h"
#include "include/releve.h"
#include "include/commande.h"

int main()
{
    FT_HANDLE ftHandle = NULL;
    FT_STATUS ftStatus;

    // Open USB device (index 0)
    ftStatus = FT_Open(0, &ftHandle);
    if (ftStatus != FT_OK)
    {
        fprintf(stderr, "Impossible d'ouvrir le périphérique USB (FT_Open: %lu)\n", ftStatus);
        return EXIT_FAILURE;
    }
    printf("Périphérique USB ouvert avec succès.\n");

    temp_t maTemp = {.interieure = 0.0, .exterieure = 0.0};
    float maConsigne = 19.0;
    float puis = 0.0;
    int regul = 2; // 1 = tout ou rien, 2 = PID

    // PID persistent state
    float erreur_prec = 0.0;
    float int_prec = 0.0;
    bool isFirst = true;

    printf("Démarrage de la supervision thermique USB...\n");

    while (1)
    {
        // Read temperatures from STM32 over USB (6 bytes)
        releve(&maTemp, ftHandle);

        // Update setpoint from file if changed
        maConsigne = consigne(maConsigne);

        // Safety stop if setpoint drops to or below 5°C
        if (maConsigne <= 5.0)
        {
            puis = 0.0;
            commande(&puis, ftHandle);
            visualisationC(puis);
            printf("Consigne à %.1f°C (<= 5°C), arrêt du chauffage. Fin de programme.\n", maConsigne);
            break;
        }

        // Compute power depending on chosen regulation mode
        if (regul == 1)
        {
            puis = regulation_1ou0(maConsigne, maTemp.interieure);
        }
        else if (regul == 2)
        {
            puis = regulation_PID(maConsigne, maTemp.interieure, &erreur_prec, &int_prec, &isFirst);
            isFirst = false;
        }
        else
        {
            printf("Mode de régulation invalide (%d). Utiliser 1 (tout-ou-rien) ou 2 (PID).\n", regul);
            puis = 0.0;
        }

        // Send power command (0-100%) to STM32 over USB (1 byte)
        commande(&puis, ftHandle);

        // Update visualisation files
        visualisationT(&maTemp);
        visualisationC(puis);

        printf("Consigne: %.2f°C | Int: %.2f°C | Ext: %.2f°C | Cmd: %.1f%%\n",
               maConsigne, maTemp.interieure, maTemp.exterieure, puis);

        sleep(1);
    }

    FT_Close(ftHandle);
    return EXIT_SUCCESS;
}