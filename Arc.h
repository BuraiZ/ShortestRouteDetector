#ifndef _Arc_H
#define _Arc_H

#include <iostream>
#include "Sommet.h"

using namespace std;

class Arc {
	public:
		Arc();
		Arc(int distance);
		int getDistance();
	private:
		int _distance;


};

#endif // _Arc_H
