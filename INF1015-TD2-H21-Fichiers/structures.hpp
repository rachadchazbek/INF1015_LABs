#pragma once
// Structures mémoires pour une collection de films.
using namespace std;
#include <iostream>
#include <string>
#include <memory>
#include <functional>

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.


// classe ListeFilms
class ListeFilms {
public:
	// toutes les fonction liées avec notre classe
	ListeFilms();
	ListeFilms(int capa, int nElem, Film** elem);
	void ajouterFilm(Film film);
	void enleverUnFilm(Film* pointeur_film);
	Acteur* trouverActeur(string acteur_nom) const;
	void detruireFilm(Film film);
	void detruireListeDeFilms();
	void afficherActeurs(Film film) const;
	void afficherActeur(const Acteur& acteur) const;
	void afficherListeFilms() const;
	void afficherFilmographieActeur(const string& nomActeur) const;
	Film* operator[](const int& i);
	bool trouver(Film f, const function<bool(Film)>& critere);
	const int& getNElements() {return nElements;}
        Film** getElements() {return elements;}
	
private:
	// les atributs de la classe
	int capacite, nElements;
	Film** elements;
};

// constructeur par défaut de notre classe
ListeFilms::ListeFilms() {
    this->capacite = 1;    
    this->nElements = 0;
    this->elements = nullptr;
}

// le constructeur de notre classe
ListeFilms::ListeFilms(int capa, int nElem, Film** elem) {
	this->capacite = capa;
	this->nElements = nElem;
	this->elements = elem;
}

// surcharge d'opérateur pour []
Film* ListeFilms::operator[](const int& i) {
	return (*(elements + i));   // cet opérateur nous retourne un pointeur de Film de tel sorte que on peut écrire : Film nomFilm = *listeFilms[i] 
                                    // pour la section Chapitres 7 – 8 du TD3
}


// classe ListeActeurs
class ListeActeurs {
public : 
	ListeActeurs();
	ListeActeurs(int capacite, int nElements, std::unique_ptr<std::shared_ptr<Acteur>[]> element);
        std::unique_ptr<std::shared_ptr<Acteur>[]> getElements() {return move(elements);}
	
private:
	int capacite, nElements;
	std::unique_ptr<std::shared_ptr<Acteur>[]> elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};

// constructeur par défaut de notre classe
ListeActeurs::ListeActeurs() {
	this->capacite = 100;
	this->nElements = 0;
	this->elements = nullptr;
}

// constructeur de notre classe
ListeActeurs::ListeActeurs(int capacite, int nElements, std::unique_ptr<std::shared_ptr<Acteur>[]>element) {
	this->capacite = capacite;
	this->nElements = nElements;
	this->elements = move(element);
}


// classe Film
class Film
{
public :
	Film();
	Film(string titre, string realisateur, ListeActeurs acteurs);
	friend ostream& operator<<(ostream& os, const Film& film);
	void setTitre(string s) {titre = s;}
        string getTitre() {return titre;}
        void setActeurs(ListeActeurs s) {acteurs = move(s);}
        ListeActeurs getActeurs() {return move(acteurs);}
        Film(const Film &obj);

private :
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

// constructeur par défaut de notre classe
Film::Film() {
 this->titre = "";
 this->realisateur = "";
 ListeActeurs listeActeurs;
 this->acteurs = move(listeActeurs);
 }

// constructeur de notre classe
Film::Film(string titre, string realisateur, ListeActeurs acteurs) {
this->titre = titre;
this->realisateur = realisateur;
this->acteurs = move(acteurs);
}

// surcharge d'opérateur de cout
ostream& operator<<(ostream& os, const Film& film)
{
	os << film.titre << " ";
	return os;                  // l'opérateur "cout <<" nous retourne le titre du Film en question, pour la section Chapitre 7 : du TD3
}

//  chercher un film en lui passant une lambda pour indiquer le critère
bool ListeFilms::trouver(Film f, const function<bool(Film)>& critere) {
      return critere(move(f));
}

// copy-constructor de notre classe
Film::Film(const Film &obj) {
   cout << "Appel du copy constructor";
   this->titre = obj.titre;
   this->realisateur = obj.realisateur;
   this->anneeSortie = obj.anneeSortie;
   this->recette = obj.recette;
}

// classe Acteur
class Acteur {
public :
	Acteur();
	Acteur(string nom, int anneeNaissance, char sexe, ListeFilms joueDans);
	void setNom(string s) {nom = s;}
        const string& getNom() {return nom;}

private :
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};

// constructeur par défaut de notre classe 
Acteur::Acteur() {
	this->nom = "";
	this->anneeNaissance = 0;
	this->sexe = *"M";
	ListeFilms ListeFilms;
	this->joueDans = ListeFilms;
}

// constructeur de notre classe
Acteur::Acteur(string nom, int anneeNaissance, char sexe, ListeFilms joueDans) {
	this->nom = nom;
	this->anneeNaissance = anneeNaissance;
	this->sexe = sexe;
	this->joueDans = joueDans;
}

