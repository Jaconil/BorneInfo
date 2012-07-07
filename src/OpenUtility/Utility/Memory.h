#ifndef Memory_h
	#define Memory_h

#include <stdio.h>
#include "CodeErreur.h"

namespace OpenUtility
{

void* MyMalloc(int Size);
void* MyCalloc(size_t num,size_t size);
void* MyRealloc(void *Ptr,int Size);
char* MyStrdup(const char *strSource);
void MyFree(void *Ptr);

class CMyException
{
	// cette classe, de type abstrait, represente une ou plusieurs exceptions

	// ATTRIBUTS:
private:
	
	unsigned int valeur;	// cette variable contient la valeur de
							// l'exception.
	unsigned int type;	// cette variable donne des infos supl�mentaires
						// sur le type d'erreur rencontr�e.

	/* Etat initial
		valeur=NO_ERREUR
		type=NO_ERREUR */

	// PRIMITIVES:
public:
	
	CMyException();
	/* Constructeur par defaut de la classe
	E: n�ant
	necessite: n�ant
	S: n�ant
	entraine: l'objet est initialis� */

	CMyException(unsigned int Nvaleur);
	/* Constructeur de la classe
	E: valeur du champ valeur
	necessite: n�ant
	S: n�ant
	entraine: l'objet est initialis� */

	CMyException(unsigned int Nvaleur,unsigned int Ntype);
	/* Constructeur de la classe
	E: valeur du champ valeur, valeur du champ type
	necessite: n�ant
	S: n�ant
	entraine: l'objet est initialis� */

	~CMyException(){}
	/* Destructeur par defaut de la classe
	E: n�ant
	necessite: n�ant
	S: n�ant
	entraine: l'objet est d�truit */

	void modifier_valeur(unsigned int valeur);
	/* Cette fonction permet de modifier la valeur de l'exception
	E: la nouvelle valeur
	necessite: n�ant
	S: n�ant
	entraine: la valeur de l'exception est retourn�e */

	int lire_valeur();
	/* Cette fonction permet de consulter la valeur de l'exception
	E: n�ant
	necessite: n�ant
	S: la valeur de l'exception
	entraine: la valeur de l'exception est retourn�e */

	void modifier_type(unsigned int valeur);
	/* Cette fonction permet de modifier la valeur de type
	E: la nouvelle valeur
	necessite: n�ant
	S: n�ant
	entraine: la valeur de type est retourn�e */

	int lire_type();
	/* Cette fonction permet de consulter la valeur de type
	E: n�ant
	necessite: n�ant
	S: la valeur de la ligne d'erreur
	entraine: la valeur de type est retourn�e */

	//MODULES EXTERNES
};

}

#endif
