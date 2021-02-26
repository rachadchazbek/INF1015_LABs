#pragma once
// Structures mémoires pour une collection de films.
using namespace std;
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include "gsl/span"
using namespace gsl;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.


// classe ListeFilms
class ListeFilms {
public:
	// toutes les fonction liées avec notre classe
	ListeFilms(); // constructeur par défaut de notre classe
	ListeFilms(int capa, int nElem, Film** elem); // le constructeur de notre classe
	void changeDimension(int nouvelleCapacite);
	void ajouterFilm(Film* film);
	span<Film*> enSpan() const;
	void enleverFilm(const Film* film);
	Acteur* trouverActeur(const string& nomActeur) const;
	void detruire(bool possedeLesFilms);
	Film* operator[](const int& i); // surcharge d'opérateur pour []
	bool trouver(Film f, const function<bool(Film)>& critere );
	int& getNElements() const {return nElements;} // getter de nElements
        Film** getElements() const {return elements;} // getter de elements
	
private:
	// les atributs de la classe
	int capacite, nElements;
	Film** elements;
};


// classe ListeActeurs
class ListeActeurs {
public : 
	ListeActeurs();
	ListeActeurs(int capacite, int nElements, std::unique_ptr<std::shared_ptr<Acteur>[]> element);
        std::unique_ptr<std::shared_ptr<Acteur>[]> getElements() const {return move(elements);} // getter de elements
	void setElements(std::unique_ptr<std::shared_ptr<Acteur>[]> s) {elements = move(s);} // setter de elements
	
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
	Film(); // constructeur par défaut de notre classe
	Film(string titre, string realisateur, ListeActeurs acteurs); // constructeur de notre classe
	friend ostream& operator<<(ostream& os, const Film& film); // surcharge d'opérateur de cout
	void setTitre(string s) {titre = s;} // setter de titre
        string getTitre() const{return titre;} // getter de titre
        void setActeurs(ListeActeurs s) {acteurs = move(s);} //setter de acteurs
        ListeActeurs getActeurs() const {return move(acteurs);} // getter de acteurs
        Film(const Film &obj); // copy-constructor
	int getRecette() const {return recette;} // getter de la recette

private :
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};


// classe Acteur
class Acteur {
public :
	Acteur(); // constructeur par défaut de notre classe 
	Acteur(string nom, int anneeNaissance, char sexe, ListeFilms joueDans); // constructeur de notre classe
	void setNom(string s) {nom = s;} // setter de nom
        string getNom() const {return nom;} // getter de nom
	ListeFilms getJoueDans() const {return joueDans;} // getter de joueDans
	void setAnneeNaissance(int s) {anneeNaissance = s;} // setter de anneeNaissance

private :
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};
