// Solutionnaire du TD3 INF1015 hiver 2021
// Par Francois-R.Boyer@PolyMtl.ca

#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "td4.hpp"      // Structures de données pour la collection de films en mémoire.

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>
#include <vector>   
#include <memory>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).
using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{

UInt8 lireUint8(istream& fichier)
{
	UInt8 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
UInt16 lireUint16(istream& fichier)
{
	UInt16 valeur = 0;
	fichier.read((char*)&valeur, sizeof(valeur));
	return valeur;
}
string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUint16(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}

// Fonctions pour ajouter un Film à une ListeFilms.
//[
void ListeFilms::changeDimension(int nouvelleCapacite)
{
	Film** nouvelleListe = new Film*[nouvelleCapacite];
	
	if (elements != nullptr) {  // Noter que ce test n'est pas nécessaire puique nElements_ sera zéro si elements_ est nul, donc la boucle ne tentera pas de faire de copie, et on a le droit de faire delete sur un pointeur nul (ça ne fait rien).
		nElements = min(nouvelleCapacite, nElements);
		for (int i : range(nElements))
			nouvelleListe[i] = elements[i];
		delete[] elements;
	}
	
	elements = nouvelleListe;
	capacite = nouvelleCapacite;
}

void ListeFilms::ajouterFilm(Film* film)
{
	if (nElements == capacite)
		changeDimension(max(1, capacite * 2));
	elements[nElements++] = film;
}
//]

// Fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.
//[
// On a juste fait une version const qui retourne un span non const.  C'est valide puisque c'est la struct qui est const et non ce qu'elle pointe.  Ça ne va peut-être pas bien dans l'idée qu'on ne devrait pas pouvoir modifier une liste const, mais il y aurais alors plusieurs fonctions à écrire en version const et non-const pour que ça fonctionne bien, et ce n'est pas le but du TD (il n'a pas encore vraiment de manière propre en C++ de définir les deux d'un coup).
span<Film*> ListeFilms::enSpan() const { return span(elements, nElements); }

void ListeFilms::enleverFilm(const Film* film)
{
	for (Film*& element : enSpan()) {  // Doit être une référence au pointeur pour pouvoir le modifier.
		if (element == film) {
			if (nElements > 1)
				element = elements[nElements - 1];
			nElements--;
			return;
		}
	}
}
//]

// Fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
//[

//NOTE: Doit retourner un Acteur modifiable, sinon on ne peut pas l'utiliser pour modifier l'acteur tel que demandé dans le main, et on ne veut pas faire écrire deux versions.
shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const
{
	for (const Film* film : enSpan()) {
		for (const shared_ptr<Acteur>& acteur : film->acteurs.enSpan()) {
			if (acteur->getNom() == nomActeur)
				return acteur;
		}
	}
	return nullptr;
}
//]

//constructeur par defaut de la classe Item
Item::Item() {
	this->titre = "";
	this->annee = 0;
}

// destructeur de la classe Item
Item::~Item() {
	cout << "Appel au destructeur de Item. " << "\n";
}

// Méthodes de la classe Film 
//[
// constructeur par défaut de notre classe
Film::Film()
{
	this->titre = "";
	this->annee = 0;
	this->realisateur = "";
	this->recette = 0;
	ListeActeurs listeActeurs;
	this->acteurs = move(listeActeurs);
}

// destructeur de notre classe Film
Film::~Film() {
	cout << "Appel au destructeur de Film. " << "\n";
}
//[

//[
// constructeur par defaut de la classe Livre
Livre::Livre() {
	this->titre = "";
	this->annee = 0;
	this->auteur = "";
	this->nbCopiesVendues = 0;
	this->nbPages = 0;
}

// destructeur de notre classe Livre
Livre::~Livre() {
	cout << "Appel au destructeur de Livre. " << "\n";
}
//]

// Méthodes de la classe FilmLivre
//[
// constructeur de notre classe
FilmLivre::FilmLivre(Film* film, Livre* livre) {
	this->Film::titre = film->getTitre();
	this->Film::annee = film->getAnnee();
	this->realisateur = film->getRealisateur();
	this->recette = film->getRecette();
	ListeActeurs act(film->acteurs);
	this->acteurs = move(act);
	this->auteur = livre->getAuteur();
	this->nbCopiesVendues = livre->getNbCopiesVendues();
	this->nbPages = livre->getNbPages();
}
// destructeur de la classe FilmLivre
FilmLivre::~FilmLivre() {
	cout << "Appel au destructeur de FilmLivre. " << "\n";
}
//]

// Méthodes de la classe Acteurs
//[
// constructeur par défaut de notre classe Acteurs
Acteur::Acteur()
{
	this->nom = "";
	this->anneeNaissance = 0;
	this->sexe = *"M";
	ListeFilms ListeFilms;
	this->joueDans = ListeFilms;
}

// constructeur de notre classe Acteurs
Acteur::Acteur(string nom, int anneeNaissance, char sexe, ListeFilms joueDans)
{
	this->nom = nom;
	this->anneeNaissance = anneeNaissance;
	this->sexe = sexe;
	this->joueDans = joueDans;
}
//]

// Les fonctions pour lire le fichier et créer/allouer une ListeFilms.

shared_ptr<Acteur> lireActeur(istream& fichier, const ListeFilms& listeFilms)
{
	Acteur acteur = {};
	acteur.setNom(lireString(fichier));
	acteur.setAnneeNaissance(lireUint16(fichier));
	acteur.setSexe(lireUint8(fichier));

	shared_ptr<Acteur> acteurExistant = listeFilms.trouverActeur(acteur.getNom());
	if (acteurExistant != nullptr)
		return acteurExistant;
	else {
		cout << "Création Acteur " << acteur.getNom() << endl;
		return make_shared<Acteur>(move(acteur));  // Le move n'est pas nécessaire mais permet de transférer le texte du nom sans le copier.
	}
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms)
{
	Film* film = new Film;
	film->setTitre(lireString(fichier));
	film->setRealisateur(lireString(fichier));
	film->setAnnee(lireUint16(fichier));
	film->setRecette(lireUint16(fichier));
	auto nActeurs = lireUint8(fichier);
	film->setActeurs(ListeActeurs(nActeurs));  // On n'a pas fait de méthode pour changer la taille d'allocation, seulement un constructeur qui prend la capacité.  Pour que cette affectation fonctionne, il faut s'assurer qu'on a un operator= de move pour ListeActeurs.
	cout << "Création Film " << film->getTitre() << endl;

	for ([[maybe_unused]] auto i : range(nActeurs)) {  // On peut aussi mettre nElements_ avant et faire un span, comme on le faisait au TD précédent.
		film->acteurs.ajouter(lireActeur(fichier, listeFilms));
	}

	return film;
}

ListeFilms creerListe(string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	
	int nElements = lireUint16(fichier);

	ListeFilms listeFilms;
	for ([[maybe_unused]] int i : range(nElements)) { //NOTE: On ne peut pas faire un span simple avec ListeFilms::enSpan car la liste est vide et on ajoute des éléments à mesure.
		listeFilms.ajouterFilm(lireFilm(fichier, listeFilms));
	}
	
	return listeFilms;
}

// Fonction pour détruire une ListeFilms et tous les films qu'elle contient.
//[
//NOTE: La bonne manière serait que la liste sache si elle possède, plutôt qu'on le dise au moment de la destruction, et que ceci soit le destructeur.  Mais ça aurait complexifié le TD2 de demander une solution de ce genre, d'où le fait qu'on a dit de le mettre dans une méthode.
void ListeFilms::detruire(bool possedeLesFilms)
{
	if (possedeLesFilms)
		for (Film* film : enSpan())
			delete film;
	delete[] elements;
}
//]


int main()
{
	#ifdef VERIFICATION_ALLOCATION_INCLUS
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
	#endif
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	ListeFilms listeFilms = creerListe("films.bin");
	
	// nouveau vecteur items
	vector <Item*> items;
	// mettre les elements de listeFilms dans le vecteur items
	for (int i = 0; i < listeFilms.size(); ++i) {
		Film* film = listeFilms[i];
		Item* itemf = dynamic_cast<Item*>(film);
		items.push_back(itemf);
	}

	// mettre les livres dans livres.txt dans le vecteur items
	fstream file;
	string word, filename;
	vector <string> vect;  // vecteur dans lequel on va mettre tout les éléments de livres.txt
	filename = "livres.txt";

	// opening file 
	file.open(filename.c_str());
	string ini = "";
	bool crit = 0;
	bool crite = 0;

	// ce code permet de mettre dans un vecteur vect tout les 25 éléments contenue dans la liste livres.txt, donc le premier élément de vect serait le titre du premier livre, puis le second élément 
	// 	   sera l'année de celui ci et ainsi de suite
	//[
	while (file >> word) {   // pour chaque mot qui sont séparé par un espace dans le texte
		crite = 0;
		for (int i = 1; i < word.size(); ++i) {      // si le mot contient ", crite = 0
			if (word[i] == '"') { crit = 0; ini += word; crite = 1; break; }
		}
		if (word[0] == '"' or crit == 1)   // si le premier élément du mot = ", ou crit = 1, rajouter le mot dans ini
		{
			ini += word;
			ini += " ";
			crit = 1;

		}
		else if (crite != 1)    // si crite != 1, push back le mot dans le vecteur
		{
			vect.push_back(word);
		}
		  
		if (crit == 0 and ini.size() > 1) {   // si crit == 0 et ini.size() > 1, push back ini dans le vecteur
			ini.erase(remove(ini.begin(), ini.end(), '"'), ini.end());  // enlever les guillemets dans ini
			vect.push_back(ini);
			ini = "";   // initialiser ini
		}
	}
	//] 
	// ce code permet de prendre tout les élément de vect, puis de créer des nouveaux livres avec
	for (int i = 0; i < 25; i = i + 5) {   // créer cinq nouveau livres, avec les éléments que on viens de stocker dans le vecteur vect
		Livre* livre = new Livre;
		livre->setTitre(vect[i]);
		livre->setAnnee(stoi(vect[i + 1]));
		livre->setAuteur(vect[i + 2]);
		livre->setNbCopiesVendues(stoi(vect[i + 3]));
		livre->setNbPages(stoi(vect[i + 4]));
		Item* iteml = dynamic_cast<Item*>(livre);
		items.push_back(iteml);                     // push back le nouveau livre dans items
	}

	// Question 4 du TD, convertir en FilmListe le film et le livre The Hobbit.
	int j;
	Film* film;
	Livre* livre;
	for (int i = 0; i < items.size(); ++i) {   // comme dans la structure de items, les films sont avant les livres, on chercher d'abord le film dont le titre est "The Hobbit"
		if (items[i]->getTitre() == "The Hobbit") {
			Film* film = dynamic_cast<Film*>(items[i]);
			j = i;                                        // on garde en mémoire la position du film, afin de plus tard commencer la recherche du livre a partir de la position i + 1
			break;
		}
	}
	for (int i = j + 1; i < items.size(); ++i) {  // on cherche ensuite le livre, a partir de la position i + 1
		if (items[i]->getTitre() == "The Hobbit") {
			Livre* livre = dynamic_cast<Livre*>(items[i]);
			break;
		}
	}

	FilmLivre* hobbit = new FilmLivre(film, livre);  // appel au constructeur de FilmLivre
	Item* item = dynamic_cast<Item*>(hobbit);
	items.push_back(item);                           // on push back dans items ce nouveau FilmLivre

	// affichage du premier film
	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	// Le premier film de la liste.  Devrait être Alien.
	cout << *items[0];

	cout << ligneDeSeparation << "Les films sont:" << endl;
	// Affiche la liste des films.  Il devrait y en avoir 7.
	for (int i = 0; i < listeFilms.size(); ++i) {   // i < listeFilms.size() car on veut chercher que les films dans items et pas les livre, et le nombre de film que on avait ajouté dans items = listeFilms.size()
		cout << *items[i] << " ";
	}

	// changer date de naissance de Benedict Cumberbatch
	for (int i = 0; i < listeFilms.size(); ++i) {
		Film* p = dynamic_cast<Film*>(items[i]);
		if ((p->acteurs)[i]->getNom() == "Benedict Cumberbatch") {
			(p->acteurs)[i]->setAnneeNaissance(1976);
		}
	}

	// Détruit et enlève le premier film de la liste (Alien).
	delete items[0];
	/*listeFilms.enleverFilm(p);*/
	int nombreFilms = listeFilms.size() - 1;

	// on affiche la liste de films dans items
	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	// Affiche la liste des films.  Il devrait y en avoir 7.
	for (int i = 0; i < nombreFilms; ++i) {        // comme nous avons enlever un film le nombre de films, sont maintenant = listeFilms.size() - 1
		cout << *items[i] << " ";
	}

	// on affiche la liste des Films dans lequel Bennedict Cumberbatch joue
	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	for (int i = 0; i < nombreFilms; ++i) {
		Film* p = dynamic_cast<Film*>(items[i]);
		if ((p->acteurs)[i]->getNom() == "Benedict Cumberbatch") {
			for (int i = 0; i < (p->acteurs)[i]->getJoueDans().size(); ++i) {
				cout << *((p->acteurs)[i]->getJoueDans()[i]);
			}
		}
	}

	// Pour une couverture avec 0% de lignes non exécutées:
	for (int i = 0; i < nombreFilms; ++i) {
		if (items[i] == nullptr) {                    // Enlever un film qui n'est pas dans la liste (clairement que nullptr n'y est pas).
			delete items[i];
		}
	}

	// Détruire tout avant de terminer le programme.
	for (int i = 0; items.size(); ++i) {
		delete items[i];
	}
	return 0;
}
