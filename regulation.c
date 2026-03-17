    #include <stdio.h>
	#include <stdlib.h>
    #include "regulation.h"
	#include "define.h"
	 
	float regulationTest(int regul,float csgn,float* tabT, int nT){
		float cmd = 0;

		if (nT <= 0 || tabT == NULL){ // pas de température et tableau de température vide
			printf("Aucune valeur de température dans le tableau\n");
			return cmd;
		}
		
		// MODE 1 : TOUT OU RIEN
		if (regul == 1){
			if (tabT[nT-1] < csgn){ // si température précédente < consigne
				cmd = 50; // on chauffe 
			}
			else{ // si température > consigne 
				cmd = 0; // on chauffe pas 
			}
		}

		// MODE 2 : PID
		else if (regul == 2){
			static float erreur_prec = 0.0;

			float erreur = csgn - tabT[nT-1]; // erreur actuelle

			float intermedI = ((((erreur - erreur_prec) * dt) / 2.0) + (erreur_prec * dt));
			float intermedD = ((erreur - erreur_prec) / dt);

			float P = KP*erreur; // proportionnel
			float I = KI*intermedI; // intégral
			float D = KD*intermedD; // dérivé

			cmd = P+I+D;

			// sécurité : si cmd ne se situe plus entre 0 et 100 
			if (cmd>100){
				cmd = 100;
			}
			else if (cmd<0){
				cmd = 0;
			}

			erreur_prec = erreur; // l'erreur actuelle devient l'erreur précédent pour le prochain calcul 
		}

		else{
			printf("Erreur ! Veuillez selectionner 1 pour un mode de chauffage tout ou rien ou 2 pour le mode de chauffage par PID.");
		}

		return cmd;
	}
    
  
