#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <memory>

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

struct ListeFilms {
	int capacite, nElements;
	Film** elements; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
};

class ListeActeurs {
public : 
	ListeActeurs();
	ListeActeurs(int capacite, int nElements, std::unique_ptr<std::unique_ptr<Acteur>[]> elements);
private:
	int capacite, nElements;
	std::unique_ptr<std::unique_ptr<Acteur>[]> elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};

ListeActeurs::ListeActeurs() {
	this->capacite = 100;
	this->nElements = 0;
	this->elements = nullptr;
}

ListeActeurs::ListeActeurs(int capacite, int nElements, std::unique_ptr<std::unique_ptr<Acteur>[]>elements) {
	this->capacite = capacite;
	this->nElements = nElements;
	this->elements = move(elements);
}

 class Film
{
public :
	Film();
	Film(string titre, string realisateur, ListeActeurs acteurs);

private :
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

Film:Film() {
 this->titre = "";
 this->realisateur = "";
 this->acteurs = ListeActeurs ListeActeurs;
 }

Film:Film(string titre, string realisateur, ListeActeurs acteurs) {
this->titre = titre;
this->realisateur = realisateur;
this->acteurs = acteurs;
}


struct Acteur
{
public :
	Acteur();
	Acteur(string nom, int anneeNaissance, char sexe, ListeFilms joueDans);

private :
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};

Acteur::Acteur() {
	this->nom = "";
	this->anneeNaissance = 0;
	this->sexe = "";
	this->joueDans = ListeFilms ListeFilms(1, 0, nullptr)
}

Acteur::Acteur(string nom, int anneeNaissance, char sexe, ListeFilms joueDans) {
	this->nom = nom;
	this->anneeNaissance = anneeNaissance;
	this->sexe = sexe;
	this->joueDans = joueDans;
}
