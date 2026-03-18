#include "include/consigne.h"

float consigne(float csgn)
{

   float thermostat_f = csgn;

   // Check for lock file to prevent simultaneous access
   FILE *verrou = fopen(".verrouConsigne", "r");
   if (verrou != NULL)
   {
      fclose(verrou);
      return csgn;
   }

   // Create lock file to indicate we're reading the consigne
   verrou = fopen(".verrouConsigne", "w");
   if (verrou == NULL)
   {
      return csgn;
   }
   fclose(verrou);

   // Read the consigne value from the file
   FILE *fichier = fopen("consigne.txt", "r");
   if (fichier != NULL)
   {
      if (fscanf(fichier, " %f", &thermostat_f) != 1)
      {
         thermostat_f = csgn;
      }
      fclose(fichier);
   }
   remove(".verrouConsigne");
   return thermostat_f;
}
