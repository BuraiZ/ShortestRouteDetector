#include "Sommet.h"
#include <string>

using namespace std;

/*
* Constructeur par defaut
*/
Sommet::Sommet() {

}

/*
* Constructeur par parametres
* @param identifiant l'identifiant du sommet
* @pram type le type de station de plein du sommet
*/
Sommet::Sommet(string identifiant, string type) : 
	_identifiant(identifiant), _type(type){

}

/*
* Afficher les donnes du sommet
*/
void Sommet::afficher() {
	cout << "identifiant: " + _identifiant  << endl;
}

/*
* Obtenir l'identifiant du sommet
* @return l'identifiant du sommet
*/
string Sommet::getIdentifiant() {
	return _identifiant;
}

/*
* Obtenir le type de station de plein du sommet
* @return le type de station
*/
string Sommet::getType() {
	return _type;
}