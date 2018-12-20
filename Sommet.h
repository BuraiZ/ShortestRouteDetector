#ifndef _Sommet_H
#define _Sommet_H
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Sommet {
	public:
		Sommet();
		Sommet(string identifiant, string type);
		void afficher();

		string getIdentifiant();
		string getType();
	private:
		string _identifiant;
		string _type;
		


};

#endif // _Sommet_H