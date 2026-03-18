#include "include/visualisationT.h"

void visualisationT(temp_t *maTemperature)
{
    FILE *file;

    if (!(access(".verrouData", F_OK) == 0)) // Check for lock file to prevent simultaneous access, false if the file does not exist, true if it exists
    {
        file = fopen(".verrouData", "w"); // cest pour dire que le fichier data.txt est en cours d'utilisation
        fclose(file);
        char heaterState[8]; // false\n\0

        // we read first the current state of the heater and the temperatures to not overwrite them when we write the new state of the heater
        file = fopen("data.txt", "r");
        fgets(heaterState, sizeof(heaterState), file);
        fclose(file);

        file = fopen("data.txt", "w");
        fputs(heaterState, file);
        fprintf(file, "%.2f\n", maTemperature->interieure);
        fprintf(file, "%.2f\n", maTemperature->exterieure);
        fclose(file);

        remove(".verrouData");
    }
}
