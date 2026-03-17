#include "visualisationT.h"

FILE *file;

void visualisationT(temp_t maTemperature)
{
    if (access(".verrouData", F_OK) == 0)
    {
        file = fopen(".verrourData", "w"); // cest pour dire que le fichier data.txt est en cours d'utilisation
        fclose(file);

        file = fopen("data.txt", "w");

        fprintf(file, "%.2f ");

        fclose(file);
    }
}
