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
    FT_HANDLE ftHandle = NULL; // void* — no extra star needed
    FT_STATUS ftStatus;        // unsigned long — no star needed

    ftStatus = FT_Open(0, &ftHandle);
    if (ftStatus != FT_OK)
    {
        fprintf(stderr, "Impossible d'ouvrir le peripherique USB (FT_Open: %lu)\n", ftStatus);
        return EXIT_FAILURE;
    }
    printf("Peripherique USB ouvert avec succes.\n");

    temp_t maTemp = {.interieure = 0.0, .exterieure = 0.0};
    float maConsigne = 19.0;
    float puis = 0.0;
    int regul = 2;

    float erreur_prec = 0.0;
    float int_prec = 0.0;
    bool isFirst = true;

    printf("Demarrage de la supervision thermique USB...\n");

    while (1)
    {
        releve(&maTemp, ftHandle);

        maConsigne = consigne(maConsigne);

        if (maConsigne <= 5.0)
        {
            puis = 0.0;
            commande(&puis, ftHandle);
            visualisationC(puis);
            printf("Consigne a %.1f C (<= 5 C), arret du chauffage.\n", maConsigne);
            break;
        }

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
            printf("Mode invalide.\n");
            puis = 0.0;
        }

        commande(&puis, ftHandle);
        visualisationT(&maTemp);
        visualisationC(puis);

        printf("Consigne: %.2f C | Int: %.2f C | Ext: %.2f C | Cmd: %.1f%%\n",
               maConsigne, maTemp.interieure, maTemp.exterieure, puis);

        sleep(1);
    }

    FT_Close(ftHandle);
    return EXIT_SUCCESS;
}