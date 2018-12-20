#ifndef _Vehicule_H
#define _Vehicule_H

#include <iostream>
#include "Sommet.h"

using namespace std;

class Vehicule {
public:
	Vehicule();
	Vehicule(string type, int distanceMax, int distanceActuelle);
	int getDistanceMax();
	int getDistanceActuelle();
	string getType();
	void setDistanceMax(int d);
	void setDistanceActuelle(int d);

private:
	string _typeVehicule;
	int _distanceMax;
	int _distanceActuelle;
};

#endif // _Vehicule_H