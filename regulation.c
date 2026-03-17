    #include <stdio.h>
	#include <stdlib.h>
    #include "regulation.h"
	#include "define.h"
	 
	float regulationTest(int regul,float consigne,float* tabT, int nT){
		float cmd = 0;

		if (nT <= 0 || tabT == NULL){ // pas de température et tableau de température vide
			printf("Aucune valeur de température dans le tableau\n");
			return cmd;
		}
		
		// MODE 1 : TOUT OU RIEN
		if (regul ==1){
			if (tabT[nT-1] < consigne){ // si température précédente < consigne
				cmd = 50; // on chauffe 
			}
			else{ // si température > consigne 
				cmd = 0; // on chauffe pas 
			}
		}

		// MODE 2 : PID
		else if (regul == 2){
			static float erreur_prec = 0.0;
		}

		return cmd;
	}
    
  
