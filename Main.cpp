#include "Graphe.h"
#include "Vehicule.h"
#include "Arc.h"
#include "Sommet.h"
#include <iostream>
#include <fstream>
#include <string>
#include<string>
#include<stack>

using namespace std;

vector<pair<string, vector<Next>>> _myDataExctract;
/*
* Transformer le caractere en majuscule
* @param chr le caractere
* @ return boolean
*/
bool upper(char chr)
{
	return chr >= 'A' && chr <= 'Z'; // same as return chr >= 65 && chr <= 90
}

/*
* Effacer les sommets dans le graphe extrait qui n'ont pas de sommet relié
*/
void cleanGrapheExtrait() {
	for (int i = 0; i < _myDataExctract.size(); i++) {
		string source = _myDataExctract.at(i).first;
		vector<Next> vecNext = _myDataExctract.at(i).second;
		if (vecNext.size() <= 0) {
			_myDataExctract.erase(_myDataExctract.begin() + i);
			i--;
		}
	}
}


/*
* permet de creer le graphe representant les routes et les stations de chargement a partir d’un ?chier,
* dont le nom est passe en parametre.
*/
Graphe creerGraphe(string nomFichier) {

	Graphe graphe = Graphe();

	ifstream myfile(nomFichier);
	string line;

	//Vérifier la lecture du fichier
	if (myfile.is_open()) {

		//lire la première ligne du fichier (sommet)
		getline(myfile, line);

		string tempWord = "";
		Sommet sommetTemp;
		string identifiant = "";
		string type = "";
		// Station,type;
		for (unsigned int i = 0; i < line.length(); i++) {
			if (upper(line.at(i))) {
				identifiant += line.at(i);
				i++;
			}
			else if (line.at(i) == ';') {
				//Sommet info list
				sommetTemp = Sommet(identifiant, type);
				graphe.ajouterSommetInfo(sommetTemp);
				//Clean
				type = "";
				identifiant = "";
			}
			else {
				type += line.at(i);
			}
		}





		//Lire la deuxième ligne du fichier (arc)
		getline(myfile, line);

		string source = "";
		string destination = "";
		int length = 0;
		bool isSource = true;
		for (unsigned int i = 0; i < line.length(); i++)
		{
			if (isalpha(line.at(i)))
			{
				if (isSource)
				{
					source = line.at(i);
					isSource = false;
				}

				else
				{
					destination = line.at(i);
					isSource = true;
				}
			}

			else if (isdigit(line.at(i)))
			{
				string distance = "";

				while (line.at(i) != ';')
				{
					distance += line.at(i);
					i++;
				}


				Arc arc = Arc(stoi(distance));
				Sommet sommet = graphe.getSommet(destination);
				Next next = Next(sommet, arc);
				graphe.ajouterSommetMap(source, next);
			}

		}

		myfile.close();
	}
	else {
		cout << "erreur d'ouverture du fichier" << endl;
	}

	return graphe;
}

/*
* permet d’afficher le graphe
*/
void lireGraphe(Graphe graphe) {
	graphe.afficherGraphe();
}

/*
* Verifier que la destination est valide
* @param destination le sommet destination
* @param vehicule le vehicule du graphe
* @return boolean
*/
bool destinationValide(Next destination, Vehicule vehicule)
{
	int distanceRestante = vehicule.getDistanceActuelle();
	if (destination._arc.getDistance() <= distanceRestante)
		return true;
	else return false;
}

/*
* permet d’extraire le sous-graphe r´esultant d’un graphe contenant uniquement les routes empruntables
* suivant l’autonomie maximale du v´ehicule
*/
bool extraireGraphe(Vehicule v, string source, string destination, Graphe graphe)
{
	int index;
	bool rienTrouver = false;
	if (graphe.find(source, index)) {
		vector<Next> vecNext = graphe.getAllData().at(index).second;
		string type;
		for (int k = 0; k < graphe.getSommetVectorInfo().size(); k++) {
			if (graphe.getSommetVectorInfo().at(k).getIdentifiant() == source) {
				type = graphe.getSommetVectorInfo().at(k).getType();
				k = graphe.getSommetVectorInfo().size();
			}
		}
		if (type != "rien" && (v.getType() == type || type == "hybrid" || v.getType() == "hybrid")) {
			v.setDistanceActuelle(v.getDistanceMax());
		}

		_myDataExctract.push_back(make_pair(source, vecNext));
		int fin = _myDataExctract.size() - 1;
		vector<Next> vecNextNewData = _myDataExctract.at(fin).second;
		for (int i = 0; i < vecNextNewData.size(); i++) {

			if (vecNextNewData.at(i)._sommetDestination.getIdentifiant() == destination && destinationValide(vecNext.at(i), v)) {
				//return true;
				rienTrouver = true;
			}
			else if (destinationValide(vecNextNewData.at(i), v)) {
				Vehicule vNew = v;
				vNew.setDistanceActuelle(v.getDistanceActuelle() - vecNextNewData.at(i)._arc.getDistance());
				string newSource = vecNextNewData.at(i)._sommetDestination.getIdentifiant();
				if (extraireGraphe(vNew, newSource, destination, graphe)) {
					rienTrouver = true;
				}
				else {
					vecNextNewData.erase(vecNextNewData.begin() + i);
					_myDataExctract.at(fin).second = vecNextNewData;
					i--;
				}
			}
			else {
				vecNextNewData.erase(vecNextNewData.begin() + i);
				_myDataExctract.at(fin).second = vecNextNewData;
				i--;
			}
		}
	}
	else {
		return false;
	}
	return rienTrouver;
}


/*
* permet de d´eterminer, `a l’aide de l’algorithme de Dijkstra,
* le plus court chemin entre deux points dans un graphe,
* avec l’origine et la destination pass´es en param`etres.
* La fonction affiche l’autonomie ?nale restante du v´ehicule,
* le plus court chemin utilis´e et la longueur de ce dernier,
* s’il existe, ou affiche un message d’erreur, dans le cas contraire.
* @param vehicule le vehicule du graphe
* @param graphe un graphe
* @param source l'identifiant du sommet source
* @param destination l'identifiant du sommet destination
* @return la longueur du chemin le plus court
*/
int plusCourtChemin(Vehicule& vehicule, const Graphe graphe, const string& source, const string& destination) {

	Graphe grapheCopy = graphe;

	//Extraire graphe pour obtenir un sous-graphe qui ne contient
	//que les chemins accessibles. Lalgorithme de Dijstra 
	//est ensuite apppliquee sur ce graphe et est donc
	//simplifiee
	extraireGraphe(vehicule, source, destination, grapheCopy);
	cleanGrapheExtrait();
	Graphe grapheExtrait = Graphe(_myDataExctract, grapheCopy.getSommetVectorInfo());

	if (grapheExtrait.getAllData().size() == 0)
	{
		cout << "Erreur: Aucun chemin trouve." << endl << endl;
		return vehicule.getDistanceActuelle();
	}

	cout << " __________________________________________" << endl;
	cout << "Affichage du sous-graphe extrait:" << endl << endl;
	grapheExtrait.afficherGraphe();
	cout << endl;

	//Structure de donnees pour accomplir lalgorithme de Dijkstra
	vector<pair<string, int>> initialDataStructure;
	vector<pair<string, string>> shortestPathDescriptions;
	vector<pair<string, int>> shortestPathLengths;

	//Remplissage de la structure de donnee initiale pour la preparation de lagorithme de Dijkstra
	for (auto & element : grapheExtrait.getAllData())
	{

		initialDataStructure.push_back(pair<string, int>(element.first, -1));
	}
	
	for (auto & element : grapheExtrait.getAllData())
	{
		for (int i = 0; i < element.second.size(); i++)
		{
			bool contains = false;
			for (int j = 0; j < initialDataStructure.size(); j++)
			{
				if (initialDataStructure[j].first == element.second[i]._sommetDestination.getIdentifiant())
				{
					contains = true;
				}
			}

			if (!contains)
			{
				initialDataStructure.push_back(pair<string, int>(element.second[i]._sommetDestination.getIdentifiant(), -1));
			}
		}
	}

	string current = "";

	int initial = 0;

	//Algorithme de Dijkstra sur le sous-graphe extrait
	while (!(initialDataStructure.empty()))
	{
		if (current == "")
		{
			current = source;

			for (int i = 0; i < initialDataStructure.size(); i++)
			{
				if (initialDataStructure[i].first == current)
				{
					initialDataStructure[i].second = 0;
					shortestPathLengths.push_back(pair<string, int>(initialDataStructure[i].first, 0));
					shortestPathDescriptions.push_back(pair<string, string>(initialDataStructure[i].first, "null"));
					initialDataStructure.erase(initialDataStructure.begin() + i);
					break;
				}
			}
		}

		else
		{
			int fas = 0;

			for (auto& graphElement : grapheExtrait.getAllData())
			{
				if (graphElement.first == current)
				{

					if (graphElement.second.size() > 0)
					{

						for (int i = 0; i < graphElement.second.size(); i++)
						{
							for (auto& elem3 : initialDataStructure)
							{
								if (elem3.first == graphElement.second[i]._sommetDestination.getIdentifiant())
								{
									bool addDescription = true;

									bool isFound = false;
									int distanceToAdd = 0;
									for (int f = 0; f < shortestPathLengths.size(); f++)
									{
										if (shortestPathLengths[f].first == current)
										{
											isFound = true;
											distanceToAdd = shortestPathLengths[f].second;
											break;
										}
									}


									int elemTest = elem3.second;
									int distanceTest = graphElement.second[i]._arc.getDistance();
									bool toReplace = false;

									if (elem3.second == -1)
									{
										elem3.second = graphElement.second[i]._arc.getDistance() + distanceToAdd;
									}


									else if (elem3.second != -1 && (elem3.second > (graphElement.second[i]._arc.getDistance() + distanceToAdd)))
									{
										elem3.second = graphElement.second[i]._arc.getDistance() + distanceToAdd;
										toReplace = true;
									}

									bool addNeeded = true;
									for (int a = 0; a < shortestPathDescriptions.size(); a++)
									{
										if (shortestPathDescriptions[a].first == elem3.first && toReplace)
										{
											shortestPathDescriptions[a].second = current;
											addNeeded = false;
											break;
										}
										else if (shortestPathDescriptions[a].first == elem3.first)
										{
											addNeeded = false;
											break;
										}
									}
									if (addNeeded)
									{
										shortestPathDescriptions.push_back(pair<string, string>(graphElement.second[i]._sommetDestination.getIdentifiant(), graphElement.first));
									}

								}
							}
						}
					}
				}
			}

			//Extract min
			string nextCurrent = "";
			int min = std::numeric_limits<int>::max();

			for (auto& element : initialDataStructure)
			{
				if (min > element.second && element.second != -1)
				{
					min = element.second;
					nextCurrent = element.first;
				}
			}


			//Remove min from initial structure
			for (int l = 0; l < initialDataStructure.size(); l++)
			{
				if (initialDataStructure[l].first == nextCurrent)
				{
					initialDataStructure.erase(initialDataStructure.begin() + l);
					break;
				}
			}

			shortestPathLengths.push_back(pair<string, int>(nextCurrent, min));
			current = nextCurrent;
		}

	}


	std::stack<string> plusCourt;

	string curr = destination;

	//Creer plus court chemin a laide dune pile
	while (curr != source)
	{
		for (int i = 0; i < shortestPathDescriptions.size(); i++)
		{
			if (shortestPathDescriptions[i].first == curr)
			{
				plusCourt.push(shortestPathDescriptions[i].first);
				curr = shortestPathDescriptions[i].second;
				break;
			}
		}
	}

	plusCourt.push(source);

	stack<string> stackAutonomie = plusCourt;
	vector<string> sommetsChemin;

	cout << "Affichage du plus court chemin: " << endl << endl;

	//Affichage du plus court chemin
	while (!plusCourt.empty())
	{
		if (plusCourt.size() != 1)
		{
			cout << plusCourt.top() << " -> ";
		}
		else
		{
			cout << plusCourt.top();
		}
		sommetsChemin.push_back(plusCourt.top());
		plusCourt.pop();
	}

	cout << endl << endl;

	cout << "Longueur du chemin:" << endl << endl;

	//Determine la longueur du chemin
	for (int i = 0; i < shortestPathLengths.size(); i++)
	{
		if (shortestPathLengths[i].first == destination)
		{
			cout << shortestPathLengths[i].second;
			cout << endl << endl;
			break;
		}
	}

	cout << "Autonomie restante: " << endl << endl;

	int autonomieRestante = vehicule.getDistanceActuelle();
	//Determine lautonomie restante du vehicule a la fin du chemin
	while (!stackAutonomie.empty())
	{
		string curr = stackAutonomie.top();

			for (int i = 0; i < grapheExtrait.getSommetVectorInfo().size(); i++)
			{
				if (curr == grapheExtrait.getSommetVectorInfo()[i].getIdentifiant())
				{
					if (grapheExtrait.getSommetVectorInfo()[i].getIdentifiant() != "rien" 
						&& (vehicule.getType() == grapheExtrait.getSommetVectorInfo()[i].getType()
							|| grapheExtrait.getSommetVectorInfo()[i].getType() == "hybrid" || vehicule.getType() == "hybrid")) {
						autonomieRestante = vehicule.getDistanceMax();
						break;
					}					
				}
			}
		

		bool remove = false;
		for (int i = 0; i < grapheExtrait.getAllData().size() && !remove; i++)
		{
			if (grapheExtrait.getAllData()[i].first == curr)
			{
				for (int j = 0; j < grapheExtrait.getAllData()[i].second.size() && !remove; j++)
				{
					for (int k = 0; k < sommetsChemin.size() && !remove; k++)
					{
						if (grapheExtrait.getAllData()[i].second[j]._sommetDestination.getIdentifiant() == sommetsChemin[k])
						{
							autonomieRestante -= grapheExtrait.getAllData()[i].second[j]._arc.getDistance();
							remove = true;
							curr = grapheExtrait.getAllData()[i].second[j]._sommetDestination.getIdentifiant();
						}
					}
				}
			}
		}

		stackAutonomie.pop();
	}

	cout << autonomieRestante << endl << endl;

	return autonomieRestante;
}

/*
* Afficher l'interface du menu
*/
void afficherMenu()
{
	cout << "Voici le menu:" << endl << endl;
	cout << "(a) Demander les caracteristiques du vehicule." << endl;
	cout << "(b) Mettre a jour la carte." << endl;
	cout << "(c) Determiner le plus court chemin." << endl;
	cout << "(d) Quitter." << endl << endl;
}


/*
* Creer un vehicule 
*/
Vehicule creerVehicule()
{
	string typeCarburant = "";
	int autonomieMaximale = 0;
	int autonomieActuelle = 0;

	cout << "Veuillez afficher les caracteristiques du vehicule:" << endl << endl;

	cout << "Entrer le type de carburant utilise (essence, electrique, hybride): " << endl;
	cin >> typeCarburant;
	cout << endl;
	cout << "Entrer lautonomie maximale du vehicule (un entier): " << endl;
	cin >> autonomieMaximale;
	cout << endl;
	cout << "Entrer lautonomie actuelle du vehicule (un entier): " << endl;
	cin >> autonomieActuelle;
	cout << endl;
	cout << "Le vehicule a ete cree avec succes." << endl << endl;

	return Vehicule(typeCarburant, autonomieMaximale, autonomieActuelle);
}

int main() {

	afficherMenu();
	char input = ' ';
	cin >> input;


	if (input == 'd')
	{
		return 0;
	}

	bool vehiculeCree = false;
	Vehicule vehicule;
	Graphe graphe;

	while (input != 'd')
	{


		cout << endl;

		if (input == 'a')
		{
			cout << "________________________________________________________";
			cout << endl << endl;
			vehicule = creerVehicule();
			vehiculeCree = true;
			cout << endl;
			cout << "________________________________________________________" << endl << endl;
		}

		else if (input == 'b')
		{
			cout << "_________________________________________________________";
			cout << endl << endl;
			string fileInputName = "";
			cout << "Veuillez entrer le nom du fichier" << endl;
			cin >> fileInputName;
			cout << endl;
			graphe = creerGraphe(fileInputName);
			lireGraphe(graphe);
			cout << endl;
			cout << "__________________________________________________________" << endl << endl;
		}

		else if (input == 'c')
		{
			cout << "__________________________________________________________";
			cout << endl << endl;
			if (!vehiculeCree)
			{
				cout << "Lutilisateur doit choisir loption (a) avant loption (b)." << endl << endl;;
			}


			else
			{
				string pointDepart = "";
				string pointFinal = "";
				cout << "Entrer le point de depart: " << endl;
				cin >> pointDepart;
				cout << "Entrer le point de final: " << endl;
				cin >> pointFinal;
				vehicule.setDistanceActuelle(plusCourtChemin(vehicule, graphe, pointDepart, pointFinal));
			}

			cout << endl << endl;
			cout << "_____________________________________________________" << endl << endl;
		}
		else if (input == 'd') {
			return 0;
		}

		afficherMenu();
		cin >> input;
	}
}
