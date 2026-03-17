#include "regulation.h"


static float regulation_1ou0(float csgn, float temperature_int)
{
	float cmd = 0;

	if (temperature_int < csgn)
	{			  // si température précédente < consigne
		cmd = 50; // on chauffe
	}
	else
	{			 // si température > consigne
		cmd = 0; // on chauffe pas
	}

	return cmd;
}

static float regulation_PID(float csgn, float temperature_int, float *erreur_prec, float *int_prec, bool *isFirst)
{
    float intermedI;
	float intermedD;
    
	float erreur = csgn - temperature_int; // erreur actuelle
    
    if(*isFirst){
	    intermedI = 0;
	    intermedD = 0;
	} 
	else {
	    intermedI = *int_prec + ((((erreur - *erreur_prec)/dt) * 2.0) + (*erreur_prec * dt));
	    intermedD = ((erreur - *erreur_prec) / dt);
	}
    
	float P = KP * erreur;	  // proportionnel
	float I = KI * intermedI; // intégral
	float D = KD * intermedD; // dérivé
	

	float cmd = P + I + D;

	// sécurité : si cmd ne se situe plus entre 0 et 100
	if (cmd > 100)
	{
		cmd = 100;
	}
	else if (cmd < 0)
	{
		cmd = 0;
	}

	*erreur_prec = erreur; // l'erreur actuelle devient l'erreur précédent pour le prochain calcul
	*int_prec = intermedI;

	return cmd;
}


float regulationTest(int regul, float csgn, float *tabT, int nT)
{

	float cmd = 0;
	float err = 0;
	float int_avant = 0;
	bool isFirst;
	

	if (nT <= 0 || tabT == NULL)
	{ // pas de température et tableau de température vide
		printf("Aucune valeur de température dans le tableau\n");
		return cmd;
	}

	// MODE 1 : TOUT OU RIEN
	if (regul == 1)
	{
		cmd = regulation_1ou0(csgn, tabT[nT - 1]);
	}

	// MODE 2 : PID
	else if (regul == 2)
	{
	    for(int i=0; i<nT; i++){
	        if (i == 0){
	            isFirst = true;
	            cmd = regulation_PID(csgn, tabT[i], &err, &int_avant, &isFirst);
	        }
	        isFirst = false;
	        cmd = regulation_PID(csgn, tabT[i], &err, &int_avant, &isFirst);
	    }
	}

	else
	{
		printf("Erreur ! Veuillez selectionner 1 pour un mode de chauffage tout ou rien ou 2 pour le mode de chauffage par PID.\n");
	}

	return cmd;
}


