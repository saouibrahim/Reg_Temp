#include "include/visualisationC.h"

void visualisationC(float puissance)
{
   FILE *file;
   char heaterState[8];
   char temp_int[8];
   char temp_ext[8];

   if (!(access(".verrouData", F_OK) == 0)) // Check for lock file to prevent simultaneous access, false if the file does not exist, true if it exists
   {
      file = fopen(".verrouData", "w"); // cest pour dire que le fichier data.txt est en cours d'utilisation
      fclose(file);
      if (puissance == 0) // if the command is 0, we consider the heater is off, otherwise it's on
      {
         // we read first the current state of the heater and the temperatures to not overwrite them when we write the new state of the heater

         file = fopen("data.txt", "r");
         fgets(heaterState, sizeof(heaterState), file);
         fgets(temp_int, sizeof(temp_int), file);
         fgets(temp_ext, sizeof(temp_ext), file);
         fclose(file);

         strcpy(heaterState, "false\n");

         file = fopen("data.txt", "w");
         fputs(heaterState, file);
         fputs(temp_int, file);
         fputs(temp_ext, file);
         fclose(file);
      }
      else
      {
         // idem
         file = fopen("data.txt", "r");
         fgets(heaterState, sizeof(heaterState), file);
         fgets(temp_int, sizeof(temp_int), file);
         fgets(temp_ext, sizeof(temp_ext), file);
         fclose(file);

         strcpy(heaterState, "true\n");

         file = fopen("data.txt", "w");
         fputs(heaterState, file);
         fputs(temp_int, file);
         fputs(temp_ext, file);
         fclose(file);
      }
      remove(".verrouData");
   }
}
