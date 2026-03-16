#include <stdio.h>
#include <stdlib.h>
#include "consigne.h"

   float consigne(float thermostatPrec_f){

      float thermostat_f = thermostatPrec_f;

      FILE* verrou = fopen(".verrouConsigne","r");
      if (verrou != NULL){
         fclose(verrou);
         return thermostat_f;
      }

      verrou = fopen(".verrouConsigne","w");
      if (verrou == NULL){
         return thermostat_f;
      }
      fclose(verrou);

      FILE* fichier = fopen("consigne.txt","r");
      if (fichier != NULL){
         if (fscanf(fichier," %f",&thermostat_f) != 1) {
            thermostat_f = thermostatPrec_f;
         }
         fclose(fichier);
      }
      remove(".verrouConsigne");
      return thermostat_f;
   }
   
   
