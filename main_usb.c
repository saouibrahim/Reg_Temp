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

    // connect to the stm32
    ftStatus = FT_Open(0, &ftHandle);
    if (ftStatus != FT_OK)
    {
        fprintf(stderr, "Impossible d'ouvrir le peripherique USB (FT_Open: %lu)\n", ftStatus);
        return EXIT_FAILURE;
    }
    printf("Peripherique USB ouvert avec succes.\n");

    // set the params correctly
    FT_SetBaudRate(ftHandle, 115200);
    FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1,
                              FT_PARITY_NONE);
    FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0x11, 0x13);
    FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0, 0);

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

        // get the Text and Tint
        releve(&maTemp, ftHandle);

        // mettre a jour notre var laConsigne egale a la valeur voulu dans consigne.txt
        maConsigne = consigne(maConsigne);

        if (maConsigne <= 5.0) // arreté le programme
        {
            puis = 0.0;
            commande(&puis, ftHandle);
            visualisationC(puis);
            printf("Consigne a %.1f C (<= 5 C), arret du chauffage.\n", maConsigne);
            break;
        }

        if (regul == 1) // regulation tout ou rien
        {
            puis = regulation_1ou0(maConsigne, maTemp.interieure);
        }
        else if (regul == 2) // regulation PID
        {
            puis = regulation_PID(maConsigne, maTemp.interieure, &erreur_prec, &int_prec, &isFirst);
            isFirst = false;
        }
        else
        {
            printf("Mode invalide.\n");
            puis = 0.0;
        }

        // envoyer la commande de puissance au stm32
        commande(&puis, ftHandle);

        // mettre a jour les fichiers de visualisation pour le gestiontemp.exe
        visualisationT(&maTemp);
        visualisationC(puis);

        // afficher les infos dans la console
        printf("Consigne: %.2f C | Int: %.2f C | Ext: %.2f C | Cmd: %.1f%%\n",
               maConsigne, maTemp.interieure, maTemp.exterieure, puis);

        // effacer les bufferrs de reception et d'emission pour eviter les debordements
        FT_Purge(ftHandle, FT_PURGE_RX | FT_PURGE_TX);

        sleep(1); // 1s
    }

    FT_Close(ftHandle);
    return EXIT_SUCCESS;
}