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
	int getNElements() const {return nElements;} // getter de nElements
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
        std::unique_ptr<std::shared_ptr<Acteur>[]> getElements() {return move(elements);} // getter de elements
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
ListeActeurs::ListeActeurs(int capacite, int nElements, std::unique_ptr<std::shared_ptr<Acteur>[]>elements) {
	this->capacite = capacite;
	this->nElements = nElements;
	this->elements = move(elements);
}


// classe Liste
template<typename T>
class Liste {
public:
    Liste()  { // défault Constructeur
        capacity = 100;
        nElements = 0;
        elements = make_unique<std::shared_ptr<T>[]>(capacity);
    }
    T getElement(int index); // Retourne un élément. Devrait accéder l'élément avec l'index de l'array des shared pointers.
    void addElement(T element, int index); // Ajouter un élément. Devrait prendre en parametre un shared pointer et le mettre dans l'array de shared pointers. Devrait aussi vérifier la capacité de l'array. Si l'array est remplit il faudrait call doubleCapacity().
    void modifyElement(T element, int index); // Modifie un élément.

    Liste(const Liste &obj) { // Copie Constructeur, permetde faire : Liste<string> listeTextes2 = listeTextes;
        this->capacity = obj.capacity;
        this->nElements = obj.nElements;
        this->elements = make_unique<std::shared_ptr<T>[]>(capacity);
        for (int i = 0; i<obj.nElements; i++) {
            this->elements[i] = obj.elements[i];
        }
    }

private:
    int capacity, nElements; 
    unique_ptr<shared_ptr<T>[]> elements;// Unique pointer d'une array de shared pointers. Chaque shared pointer, pointe vers un element.
    void doubleCapacity();// Permet de doubler la capcité des shared pointers. Devrait etre privé.
};


template<typename T>  // Getter de element dans la liste elements.
T Liste<T>::getElement(int index) {
    cout<< *elements[index] <<endl;
    return *elements[index];
}

template<typename T> // Ajout de un élément dans la liste elements Create. Crée un nouveau shared ptr dans elements et le met dans l'index spécifié par l'index de l'ancien ptr. 
void Liste<T>::addElement(T element, int index) {
    auto ptr = make_shared<T>(element);
    elements[index] = move(ptr);
    nElements = nElements + 1;
}

template<typename T> // Modificateur d'un élément de la liste elements.
void Liste<T>::modifyElement(T element, int index) {
    *elements[index] = element;
}

struct Point { double x,y; };


// classe Film
class Film
{
public :
	Film(); // constructeur par défaut de notre classe
	Film(string titre, string realisateur, ListeActeurs acteurs); // constructeur de notre classe
	friend ostream& operator<<(ostream& os, const Film& film); // surcharge d'opérateur de cout
	void setTitre(string s) {titre = s;} // setter de titre
        string getTitre() const {return titre;} // getter de titre
        void setActeurs(ListeActeurs s) {acteurs = move(s);} //setter de acteurs
        ListeActeurs getActeurs() {return move(acteurs);} // getter de acteurs
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
