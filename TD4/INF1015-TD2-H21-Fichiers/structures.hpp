// Solutionnaire du TD3 INF1015 hiver 2021
// Par Francois-R.Boyer@PolyMtl.ca
#pragma once
// Structures mémoires pour une collection de films.
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <cassert>
#include "gsl/span"
#include <vector>
using gsl::span;
using namespace std;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	void detruire(bool possedeLesFilms = false);
	Film*& operator[] (int index) { return elements[index]; }
	Film** getElements() const { return elements; } // getter de elements
	Film* trouver(const function<bool(const Film&)>& critere) {
		for (auto& film : enSpan())
			if (critere(*film))
				return film;
		return nullptr;
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
};

template <typename T>
class Liste {
public:
	Liste() = default;
	explicit Liste(int capaciteInitiale) :  // explicit n'est pas matière à ce TD, mais c'est un cas où c'est bon de l'utiliser, pour ne pas qu'il construise implicitement une Liste à partir d'un entier, par exemple "maListe = 4;".
		capacite_(capaciteInitiale),
		elements_(make_unique<shared_ptr<T>[]>(capacite_))
	{
	}
	Liste(const Liste<T>& autre) :
		capacite_(autre.nElements_),
		nElements_(autre.nElements_),
		elements_(make_unique<shared_ptr<T>[]>(nElements_))
	{
		for (int i = 0; i < nElements_; ++i)
			elements_[i] = autre.elements_[i];
	}
	Liste(Liste<T>&&) = default;  // Pas nécessaire, mais puisque c'est si simple avec unique_ptr...
	Liste<T>& operator= (Liste<T>&&) noexcept = default;  // Utilisé pour l'initialisation dans lireFilm.

	void ajouter(shared_ptr<T> element)
	{
		assert(nElements_ < capacite_);  // Comme dans le TD précédent, on ne demande pas la réallocation pour ListeActeurs...
		elements_[nElements_++] = move(element);
	}

	// Noter que ces accesseurs const permettent de modifier les éléments; on pourrait vouloir des versions const qui retournent des const shared_ptr, et des versions non const qui retournent des shared_ptr.
	shared_ptr<T>& operator[] (int index) const { return elements_[index]; }
	span<shared_ptr<T>> enSpan() const { return span(elements_.get(), nElements_); }
	int size() { return nElements_; } // getter de nElements dans Liste

private:
	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};
using ListeActeurs = Liste<Acteur>;


class Item
{
public:
	Item(); // constructeur par default de notre classe
	void setTitre(string s) { titre = s; }  //setter de titre
	void setAnnee(int s) { annee = s; }     //setter de annee
	string getTitre() const { return titre; }  //getter de titre
	friend ostream& operator<<(ostream& os, const Item& item) {    // permet l'affichage de Item
		return item.write(os);
	};
	virtual std::ostream& write(std::ostream& os) const {          // permet l'affichage de Item
		os << "Le titre de l'item : " << titre << ". Sa date de parution : " << annee << ". " << "\n";
		return os;
	}
	virtual ~Item();   // destructeur 

protected:
	string titre;
	int annee;
};


// classe Film
class Film : virtual public Item     // on met ici virtual devant public Item, car cela va permettre de pouvoir utiliser la classe FilmLivre, qui herite de Livre + Film
{
public:
	Film(); // constructeur par défaut de notre classe
	void setTitre(string s) { titre = s; } // setter de titre
	string getTitre() const { return titre; } // getter de titre
	void setActeurs(ListeActeurs s) { acteurs = move(s); } //setter de acteurs
	int getRecette() const { return recette; } // getter de la recette
	void setRealisateur(string s) { realisateur = s; } // setter realisateur
	void setAnnee(int s) { annee = s; } // setter anneeSortie
	void setRecette(int s) { recette = s; } // setter recette
	int getAnnee() const { return annee; } // getter de annee
	string getRealisateur() const { return realisateur; } // getter de realisateur
	std::ostream& write(std::ostream& os) const override {           // permet d'afficher un Film (override la methode de Item)
		os << "Le titre du film : " << titre << ", sa date de parution : " << annee << ", le nom de son réalisateur : " << realisateur << ", sa recette : " << recette << "." << "\n";
		return os;
	}
	~Film();   // destructeur
	ListeActeurs acteurs;

	// pour le 1.5, nouvelle metod getActeurs(), qui retourne un vecteur contenant tout les acteurs de ListeActeurs acteurs
	vector<shared_ptr<Acteur>> getActeurs() {
		vector <shared_ptr<Acteur>> vect;
		for (int i = 0; i < acteurs.size(); ++i) {
			vect.push_back(acteurs[i]);
		}
		return vect;
	}

protected:
	std::string realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur)
	int recette; // Année de sortie et recette globale du film en millions de dollars
};


class Livre : virtual public Item {  // on met ici virtual devant public Item, car cela va permettre de pouvoir utiliser la classe FilmLivre, qui herite de Livre + Film
public:
	Livre();
	void setAuteur(string s) { auteur = s; }  // setter de auteur
	void setNbCopiesVendues(int s) { nbCopiesVendues = s; }   // setter de nbCopiesVendues
	void setNbPages(int s) { nbPages = s; }   // setter de nbPages
	string getAuteur() const { return auteur; }   // getter de auteur
	int getNbCopiesVendues() const { return nbCopiesVendues; }  // getter de nbCopiesVendues
	int getNbPages() const { return nbPages; }   // getter de nbPages
	std::ostream& write(std::ostream& os) const override {     // permet d'affihcer un Livre (override de la methode de Item)
		os << "Le titre du livre : " << titre << ", sa date de parution : " << annee << ", le nom de son auteur : " << auteur << ", son nombre de pages : " << nbPages << ", son nombre de copies vendues : " << nbCopiesVendues << "." << "\n";
		return os;
	}
	~Livre();  // destructeur

protected:
	string auteur;
	int nbCopiesVendues, nbPages;
};


class FilmLivre : public Film, public Livre {
public:
	FilmLivre(Film* film, Livre* livre);  // constructeur de notre classe
	std::ostream& write(std::ostream& os) const override {        // permet d'afficher un FilmLivre 
		os << "Le titre du Filmlivre : " << Film::titre << ", sa date de parution : " << Film::annee << ", le nom de son auteur : " << auteur << ", son nombre de pages : " << nbPages << ", son nombre de copies vendues : " << nbCopiesVendues << ", le nom de son réalisateur : " << realisateur << ", sa recette : " << recette << "." << "\n";
		return os;
	}
	~FilmLivre();
};


// classe Acteur
class Acteur {
public:
	Acteur(); // constructeur par défaut de notre classe 
	Acteur(string nom, int anneeNaissance, char sexe, ListeFilms joueDans); // constructeur de notre classe
	void setNom(string s) { nom = s; } // setter de nom
	string getNom() const { return nom; } // getter de nom
	ListeFilms getJoueDans() const { return joueDans; } // getter de joueDans
	void setAnneeNaissance(int s) { anneeNaissance = s; } // setter de anneeNaissance
	void setSexe(char s) { sexe = s; } // setter de sexe

private:
	std::string nom; int anneeNaissance; char sexe;
	ListeFilms joueDans;
};
