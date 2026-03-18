#include "include/regulation.h"

float regulation_1ou0(float csgn, float temperature_int)
{
	float cmd = 0;
	if (temperature_int < csgn)
	{
		cmd = 50;
	}
	else
	{
		cmd = 0;
	}
	return cmd;
}

float regulation_PID(float csgn, float temperature_int, float *erreur_prec, float *int_prec, bool *isFirst)
{
	float intermedI;
	float intermedD;

	float erreur = csgn - temperature_int;

	if (*isFirst)
	{
		intermedI = 0;
		intermedD = 0;
	}
	else
	{
		intermedI = *int_prec + ((erreur + *erreur_prec) / 2.0f) * dt;
		intermedD = (erreur - *erreur_prec) / dt;
	}

	float P = KP * erreur;
	float I = KI * intermedI;
	float D = KD * intermedD;

	float cmd = P + I + D;

	if (cmd > 100)
	{
		cmd = 100;
	}
	else if (cmd < 0)
	{
		cmd = 0;
	}

	*erreur_prec = erreur;
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
	{
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
		isFirst = true;
		for (int i = 0; i < nT; i++)
		{
			cmd = regulation_PID(csgn, tabT[i], &err, &int_avant, &isFirst);
			isFirst = false;
		}
	}
	else
	{
		printf("Erreur ! Veuillez selectionner 1 pour un mode de chauffage tout ou rien ou 2 pour le mode de chauffage par PID.\n");
	}

	return cmd;
}