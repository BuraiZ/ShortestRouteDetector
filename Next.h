#ifndef _Next_H
#define  _Next_H

#include <iostream>
#include "Sommet.h"
#include "Arc.h"

using namespace std;

class Next {
public:
	Next(Sommet destination, Arc arc);
	Sommet _sommetDestination;
	Arc _arc;
};

#endif // _Arc_H
