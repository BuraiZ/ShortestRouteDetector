#ifndef _Graphe_H
#define _Graphe_H

#include <map>
#include <iostream>
#include <vector>
#include "Arc.h"
#include "Sommet.h"
#include "Next.h"
#include "Vehicule.h"
using namespace std;

class Graphe {
	public:
		Graphe();
		Graphe(const Graphe& graphe);
		Graphe(vector<pair<string, vector<Next>>> myData, vector<Sommet> sommetInfo);

		//Fonction en lien avec les sommets
		void ajouterSommetInfo(Sommet sommet);
		void ajouterSommetMap(string sommet, Next next);
		Sommet getSommet(string identifiant);
		bool find(string source, int& index);
		//attribu geter
		vector<pair<string, vector<Next>>> getAllData();
		vector<Sommet> getSommetVectorInfo();
		//display
		void afficherGraphe();

		

	private:
		vector<pair<string, vector<Next>>> _myData;
		vector<pair<string, vector<Next>>> _myDataExctract;
		vector<Sommet> _sommetInfo;

};

#endif // _Graphe_H

