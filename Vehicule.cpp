#include "Vehicule.h"

/*
* Constructeur par defaut
*/
Vehicule::Vehicule()
{
}


/*
* Constructeur par parametres
* @param type le type de carburant que consomme le vehicule
* @param distanceMax la distance maximale que peut parcourir le vehicule en 1 plein
* @param distanceParcourue la distance que le vehicule a deja parcouru
*/
Vehicule::Vehicule(string type, int distanceMax, int distanceActuelle):_typeVehicule(type) ,_distanceMax(distanceMax), _distanceActuelle(distanceActuelle)
{
}

/*
* Obtenir la distance maximale que peut parcourir le vehicule
* @return la distance maximale
*/
int Vehicule::getDistanceMax()
{
	return _distanceMax;
}

/*
* Obtenir la distance que le vehicule a deja parcouru
* @return la distance parcourue
*/
int Vehicule::getDistanceActuelle()
{
	return _distanceActuelle;
}

/*
* Obtenir le type de carburant que consomme le vehicule
* @return le type de carburant
*/
string Vehicule::getType()
{
	return _typeVehicule;
}

/*
* Modifier la distance maximale que peut parcourir le vehicule
* @param distance la distance maximale
*/
void Vehicule::setDistanceMax(int d)
{
	_distanceMax = d;
}

/*
* Modifier la distance que le vehicule a deja parcouru
* @param distance la distance parcourue en prealable
*/
void Vehicule::setDistanceActuelle(int d)
{
	_distanceActuelle = d;
}

