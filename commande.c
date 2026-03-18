#include "include/commande.h"

void commande(float puissance, FT_HANDLE ftHandle)
{
    if (ftHandle == NULL)
        return; // verif du handle, si une connexion USB existe ou non

    // si sup à 100 ou inf à 0 on limite
    if (puissance > 100)
    {
        puissance = 100;
    }
    else if (puissance < 0)
    {
        puissance = 0;
    }

    // conversion PUISS (0 à 127) en 0% à 100%
    unsigned char PUIS = (unsigned char)((puissance * 127) * 100);
    unsigned char octet_commande = PUIS;

    // Envoi via USB et vérif du résultat, dispo sur la documentation de la bibliothèque FTD2XX
    DWORD BytesWritten;
    FT_STATUS status = FT_Write(ftHandle, &octet_commande, 1, &BytesWritten);
    if (status != FT_OK || BytesWritten != 1)
    {
        fprintf(stderr, "Erreur FT_Write : status=%d, bytes=%lu\n", status, BytesWritten);
    }
}
