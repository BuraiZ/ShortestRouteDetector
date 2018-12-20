#include "Graphe.h"
#include <algorithm>

/*
* Constructeur par defaut
*/
Graphe::Graphe(){
	
}

/*
* Constructeur par copie
* @param graphe un graphe
*/
Graphe::Graphe(const Graphe & graphe):
	_myData(graphe._myData), _sommetInfo(graphe._sommetInfo)
{
	
}

/*
* Constructeur par parametres
* @param myData vecteur de next
* @param sommetInfo vecteur de sommets
*/
Graphe::Graphe(vector<pair<string, vector<Next>>> myData, vector<Sommet> sommetInfo):
	_myData(myData), _sommetInfo(sommetInfo)
{
}

/*
* Afficher les donnees sur les sommets et ses arcs du graphe
*/
void Graphe::afficherGraphe() {
	for (auto const& element : _myData) {
		string source = element.first;
		vector<Next> vecNext = element.second;
		string type = getSommet(source).getType();
		cout << source << ", " << type << ", (";
		for ( unsigned int i = 0; i < vecNext.size(); i++) {
			string x = "";
			if (i < vecNext.size() - 1) x = ",";
			cout << vecNext.at(i)._sommetDestination.getIdentifiant() << x;
		}
		cout << ")" << endl;
	}
}

/*
* Obtenir la liste des sommets du graphe
* @return la liste des sommets
*/
void Graphe::ajouterSommetInfo(Sommet sommet) {
	_sommetInfo.push_back(sommet);
}

/*
* Obtenir la liste des arcs du graphe
* @param return la liste des arcs
*/
Sommet Graphe::getSommet(string identifiant) {
	for ( unsigned int i = 0; i < _sommetInfo.size(); i++) {
		if (_sommetInfo.at(i).getIdentifiant() == identifiant) {
			return _sommetInfo.at(i);
		}
	}
	Sommet sommet = Sommet("", "");
	return sommet;
}

/*
* Rechercher le sommet qui porte cet identifiant et retourner l'index
* @param source l'identifiant du sommet recherche
* @param index la position de ce sommet dans la liste
*/
bool Graphe::find(string source, int & index)
{
	for (int i = 0; i < _myData.size(); i++) {
		if (_myData.at(i).first == source) {
			index = i;
			return true;
		}
	}
	return false;
}

/*
* Ajouter un sommet dans la liste des sommets du graphe
* @param sommet le sommet a ajouter
*/
void Graphe::ajouterSommetMap(string sommet, Next next) {
	int k = 0;
	bool exist = false;
	for ( unsigned int i = 0; i < _myData.size(); i++) {
		if (_myData.at(i).first == sommet) {
			k = i;
			exist = true;
			i = _myData.size();
		}
	}
	if (exist) {
		_myData.at(k).second.push_back(next);		
	}
	else {
		vector<Next> vector;
		vector.push_back(next);
		_myData.push_back(make_pair(sommet, vector));
	}
}	  

/*
* Retourner la liste des sommets next
* @return la liste des sommets next
*/
vector<pair<string, vector<Next>>> Graphe::getAllData()
{
	return _myData;
}

/*
* Retourner la liste des sommets
* @return la liste des sommets
*/
vector<Sommet> Graphe::getSommetVectorInfo()
{
	return _sommetInfo;
}





