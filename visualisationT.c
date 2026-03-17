#include "visualisationT.h"

void visualisationT(temp_t maTemperature)
{
    FILE *file;

    if (!(access(".verrouData", F_OK) == 0))
    {
        file = fopen(".verrouData", "w"); // cest pour dire que le fichier data.txt est en cours d'utilisation
        fclose(file);
        char heaterState[8]; // false\n\0

        file = fopen("data.txt", "r");
        fgets(heaterState, sizeof(heaterState), file);
        fclose(file);

        file = fopen("data.txt", "w");
        fputs(heaterState, file);
        fprintf(file, "%.2f\n", maTemperature.interieure);
        fprintf(file, "%.2f\n", maTemperature.exterieure);
        fclose(file);

        remove(".verrouData");
    }
}
