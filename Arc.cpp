#include "Arc.h"

/*
* Constructeur par defaut
*/
Arc::Arc() {

}

/*
* Constructeur par parametres
* @param distance la distance de l'arc
*/
Arc::Arc(int distance): _distance(distance){

}

/*
* Obtenir la distance de l'arc
* @return la distance de l'arc
*/
int Arc::getDistance()
{
	return _distance;
}

