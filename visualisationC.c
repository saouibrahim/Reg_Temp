#include "include/visualisationC.h"

void visualisationC(float puissance)
{
   FILE *file;
   char heaterState[8];
   char temp_int[8];
   char temp_ext[8];

   if (!(access(".verrouData", F_OK) == 0))
   {
      file = fopen(".verrouData", "w"); // cest pour dire que le fichier data.txt est en cours d'utilisation
      fclose(file);
      if (puissance == 0)
      {

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
