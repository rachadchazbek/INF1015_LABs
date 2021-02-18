#pragma once
// Structures mémoires pour une collection de films.

#include <string>
#include <memory>

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.


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


private:
	// les atributs de la classe
	int capacite, nElements;
	Film** elements;
};

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

Film* ListeFilms::operator[](const int& i) {

	return (*(elements + i));
}


class ListeActeurs {
public : 
	ListeActeurs();
	ListeActeurs(int capacite, int nElements, std::unique_ptr<std::shared_ptr<Acteur>[]> element);
        std::unique_ptr<std::shared_ptr<Acteur>[]> getElements() {return move(elements);}
	
private:
	int capacite, nElements;
	std::unique_ptr<std::shared_ptr<Acteur>[]> elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};

ListeActeurs::ListeActeurs() {
	this->capacite = 100;
	this->nElements = 0;
	this->elements = nullptr;
}

ListeActeurs::ListeActeurs(int capacite, int nElements, std::unique_ptr<std::shared_ptr<Acteur>[]>element) {
	this->capacite = capacite;
	this->nElements = nElements;
	this->elements = move(element);
}


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

private :
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

Film::Film() {
 this->titre = "";
 this->realisateur = "";
 ListeActeurs listeActeurs;
 this->acteurs = move(listeActeurs);
 }

Film::Film(string titre, string realisateur, ListeActeurs acteurs) {
this->titre = titre;
this->realisateur = realisateur;
this->acteurs = move(acteurs);
}

ostream& operator<<(ostream& os, const Film& film)
{
	os << film.titre << " ";
	return os;
}


class Acteur {
public :
	Acteur();
	Acteur(string nom, int anneeNaissance, char sexe, ListeFilms joueDans);
	void setNom(string s) {nom = s;}
        string getNom() {return nom;}

private :
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};

Acteur::Acteur() {
	this->nom = "";
	this->anneeNaissance = 0;
	this->sexe = *"M";
	ListeFilms ListeFilms;
	this->joueDans = ListeFilms;
}

Acteur::Acteur(string nom, int anneeNaissance, char sexe, ListeFilms joueDans) {
	this->nom = nom;
	this->anneeNaissance = anneeNaissance;
	this->sexe = sexe;
	this->joueDans = joueDans;
}

