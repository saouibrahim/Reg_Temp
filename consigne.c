#include <stdio.h>
#include <stdlib.h>
#include "consigne.h"

   float consigne(float csgn){

      float thermostat_f = csgn;

      FILE* verrou = fopen(".verrouConsigne","r");
      if (verrou != NULL){
         fclose(verrou);
         return csgn;
      }

      verrou = fopen(".verrouConsigne","w");
      if (verrou == NULL){
         return csgn;
      }
      fclose(verrou);

      FILE* fichier = fopen("consigne.txt","r");
      if (fichier != NULL){
         if (fscanf(fichier," %f",&thermostat_f) != 1) {
            thermostat_f = csgn;
         }
         fclose(fichier);
      }
      remove(".verrouConsigne");
      return thermostat_f;
   }
   