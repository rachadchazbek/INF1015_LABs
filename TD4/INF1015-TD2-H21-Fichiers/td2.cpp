// Par Francois-R.Boyer@PolyMtl.ca

#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures_solutionnaire_td2_3.hpp"      // Structures de données pour la collection de films en mémoire.

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.

#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
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


// Méthodes de la classe ListeFilms.
//[
void ListeFilms::changeDimension(int nouvelleCapacite)
{
	Film** nouvelleListe = new Film*[nouvelleCapacite];
	
	if (elements != nullptr) {  // Noter que ce test n'est pas nécessaire puique nElements sera zéro si elements est nul, donc la boucle ne tentera pas de faire de copie, et on a le droit de faire delete sur un pointeur nul (ça ne fait rien).
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
// Voir la NOTE ci-dessous pourquoi Acteur* n'est pas const.  Noter que c'est valide puisque c'est la struct uniquement qui est const dans le paramètre, et non ce qui est pointé par la struct.
span<Acteur*> spanListeActeurs(const ListeActeurs& liste) { return span(liste.elements, liste.nElements); }

//NOTE: Doit retourner un Acteur modifiable, sinon on ne peut pas l'utiliser pour modifier l'acteur tel que demandé dans le main, et on ne veut pas faire écrire deux versions aux étudiants dans le TD2.
Acteur* ListeFilms::trouverActeur(const string& nomActeur) const
{
	for (const Film* film : enSpan()) {
		for (Acteur* acteur : spanListeActeurs(film->acteurs)) {
			if (acteur->nom == nomActeur)
				return acteur;
		}
	}
	return nullptr;
}
//]

//[
// constructeur par défaut de notre classe ListeFilms
ListeFilms::ListeFilms() 
{
    this->capacite = 1;    
    this->nElements = 0;
    this->elements = nullptr;
}
//]

//[
// le constructeur de notre classe ListeFilms
ListeFilms::ListeFilms(int capa, int nElem, Film** elem) 
{
	this->capacite = capa;
	this->nElements = nElem;
	this->elements = elem;
}
//]

//[
// surcharge d'opérateur pour []
Film* ListeFilms::operator[](const int& i)
{
	return (*(elements + i));   // cet opérateur nous retourne un pointeur de Film de tel sorte que on peut écrire : Film nomFilm = *listeFilms[i] 
                                    // pour la section Chapitres 7 – 8 du TD3
}
//]

//[
//  chercher un film en lui passant une lambda pour indiquer le critère
bool ListeFilms::trouver(Film f, const function<bool(Film)>& critere)
{
      return critere(move(f));
}
//]


// Méthodes de la classe Film 
//[
// constructeur par défaut de notre classe
Film::Film()
{
 this->titre = "";
 this->realisateur = "";
 ListeActeurs listeActeurs;
 this->acteurs = move(listeActeurs);
 }
//]

//[
// constructeur de notre classe
Film::Film(string titre, string realisateur, ListeActeurs acteurs) 
{
this->titre = titre;
this->realisateur = realisateur;
this->acteurs = move(acteurs);
}
//]

//[
// surcharge d'opérateur de cout
ostream& operator<<(ostream& os, const Film& film)
{
	os << film.titre << " ";
	return os;                  // l'opérateur "cout <<" nous retourne le titre du Film en question, pour la section Chapitre 7 : du TD3
}
//]

//[
// copy constructor de notre classe
Film::Film(const Film &obj) 
{
   this->titre = obj.titre;
   this->realisateur = obj.realisateur;
   this->anneeSortie = obj.anneeSortie;
   this->recette = obj.recette;
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
//]

//[
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

Acteur* lireActeur(istream& fichier, ListeFilms& listeFilms)
{
	Acteur acteur = {};
	acteur.nom            = lireString(fichier);
	acteur.anneeNaissance = lireUint16 (fichier);
	acteur.sexe           = lireUint8  (fichier);

	Acteur* acteurExistant = listeFilms.trouverActeur(acteur.nom);
	if (acteurExistant != nullptr)
		return acteurExistant;
	else {
		cout << "Création Acteur " << acteur.nom << endl;
		return new Acteur(acteur);
	}
}

Film* lireFilm(istream& fichier, ListeFilms& listeFilms)
{
	Film film = {};
	film.titre       = lireString(fichier);
	film.realisateur = lireString(fichier);
	film.anneeSortie = lireUint16 (fichier);
	film.recette     = lireUint16 (fichier);
	film.acteurs.nElements = lireUint8 (fichier);

	Film* filmp = new Film(film); //NOTE: On aurait normalement fait le "new" au début de la fonction pour directement mettre les informations au bon endroit; on le fait ici pour que le code ci-dessus puisse être directement donné aux étudiants dans le TD2 sans qu'ils aient le "new" déjà écrit.  On aurait alors aussi un nom "film" pour le pointeur, pour suivre le guide de codage; on a mis un suffixe "p", contre le guide de codage, pour le différencier de "film".
	cout << "Création Film " << film.titre << endl;
	filmp->acteurs.elements = std::unique_ptr<std::shared_ptr<Acteur>[]> elements(new std::unique_ptr<Acteur>[50]);

	for (Acteur*& acteur : spanListeActeurs(filmp->acteurs)) {
		acteur = lireActeur(fichier, listeFilms);
		acteur->joueDans.ajouterFilm(filmp);
	}
	return filmp;
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

// Fonctions pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Enlève aussi le film détruit des films dans lesquels jouent les acteurs.  Pour fins de débogage, les noms des acteurs sont affichés lors de leur destruction.
//[
void detruireActeur(Acteur* acteur)
{
	cout << "Destruction Acteur " << acteur->nom << endl;
	acteur->joueDans.detruire();
	delete acteur;
}

bool joueEncore(const Acteur* acteur)
{
	return acteur->joueDans.size() != 0;
}

void detruireFilm(Film* film)
{
	for (Acteur* acteur : spanListeActeurs(film->acteurs)) {
	        acteur->joueDans.enleverFilm(film);
		if (!joueEncore(acteur))
			detruireActeur(acteur);
	}
	cout << "Destruction Film " << film->titre << endl;
	delete[] film->acteurs.elements;
	delete film;
} 
//]

// Fonction pour détruire une ListeFilms et tous les films qu'elle contient.
//[
//NOTE: La bonne manière serait que la liste sache si elle possède, plutôt qu'on le dise au moment de la destruction, et que ceci soit le destructeur.  Mais ça aurait complexifié le TD2 de demander une solution de ce genre, d'où le fait qu'on a dit de le mettre dans une méthode.
void ListeFilms::detruire(bool possedeLesFilms)
{
	if (possedeLesFilms)
		for (Film* film : enSpan())
			detruireFilm(film);
	delete[] elements;
}
//]

void afficherActeur(const Acteur& acteur)
{
	cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

// Fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).
//[
void afficherFilm(const Film& film)
{
	cout << "Titre: " << film.titre << endl;
	cout << "  Réalisateur: " << film.realisateur << "  Année :" << film.anneeSortie << endl;
	cout << "  Recette: " << film.recette << "M$" << endl;

	cout << "Acteurs:" << endl;
	for (const Acteur* acteur : spanListeActeurs(film.acteurs))
		afficherActeur(*acteur);
}
//]

void afficherListeFilms(const ListeFilms& listeFilms)
{
	static const string ligneDeSeparation = //[
		"\033[32m────────────────────────────────────────\033[0m\n";
	cout << ligneDeSeparation;
	for (const Film* film : listeFilms.enSpan()) {
		afficherFilm(*film);
		cout << ligneDeSeparation;
	}
}

void afficherFilmographieActeur(const ListeFilms& listeFilms, const string& nomActeur)
{
	const Acteur* acteur = listeFilms.trouverActeur(nomActeur);
	if (acteur == nullptr)
		cout << "Aucun acteur de ce nom" << endl;
	else
		afficherListeFilms(acteur->joueDans);
}

// CHAP 10 // retourne 1 si le critère est vrai sur le film que on passe en paramètre.
// si recette = 995 milions de dollars, return bool : 1.
Film critery(ListeFilms listeFilms) {                          // on passe tout les films de listeFilms dans notre méthode "trouver".
            for (int i = 0; i < listeFilms.getNElements() ; ++i) {
                bool position = listeFilms.trouver(*(*(listeFilms.getElements() + i)), [](auto v) { if (v.getRecette() == 995) {return 1;} else {return 0;};});
    
            if (position) {return (*(*(listeFilms.getElements() + i))); break;}
            }
            return (*(*(listeFilms.getElements())));
        }

int main()
{
	#ifdef VERIFICATION_ALLOCATION_INCLUS
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
	#endif
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	ListeFilms listeFilms = creerListe("films.bin");
	
	// Chapitre 10 
	// return film si recette = 995, sachant que l'opérateur "cout <<" est surchargé pour la classe Film pour affiché le titre du film.
	cout << critery(listeFilms);
	
	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	// Le premier film de la liste.  Devrait être Alien.
	cout << *(*((listeFilms.getElements())));

	cout << ligneDeSeparation << "Les films sont:" << endl;
	// Affiche la liste des films.  Il devrait y en avoir 7.
	for (int i = 0; i < listeFilms.getNElements(); ++i) {
	        cout << *(*((listeFilms.getElements()) + i)) << " ";
	}

	listeFilms.trouverActeur("Benedict Cumberbatch")->setAnneeNaissance(1976); // trouver acteur avec le nom "Benedict Cumberbatch".

	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;  
	// Affiche la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
	for (int i = 0; i < ((listeFilms.trouverActeur("Benedict Cumberbatch"))->getJoueDans()).getNElements(); ++ i) {
	        cout << *(*(((listeFilms.trouverActeur("Benedict Cumberbatch"))->getJoueDans()).getElements() + i));
	}
	
	// CHAP 7 - 8
	//[
        Film skylien = *(listeFilms[0]);
	// copier le pointeur du premier acteur de listeFilms vers le premier acteur de la liste de skylien.
	
        ListeActeurs act;
        act = skylien.getActeurs();       // pointeur vers la liste d'Acteurs du film Skylien.
	
        ListeActeurs acte;
        acte = ((listeFilms[1])->getActeurs());     // pointeur vers la liste d'Acteurs du film listeFilms[1].
	
        std::unique_ptr<std::shared_ptr<Acteur>[]> ele;
        ele = (act.getElements());                        // pointeur vers une liste de pointeurs de Films de la ListeActeurs du film skylien.
	
        std::unique_ptr<std::shared_ptr<Acteur>[]> elem;
        elem = ((acte).getElements());                     // pointeur vers une liste de pointeurs de Films de la ListeActeurs du film listeFilms[1].
 
        (ele)[0] = (elem)[0]; 
	// modifier le nom du premier acteur chez skylien, ce qui va aussi le  modifier celui de listeFilms[1], vu que on a copié le pointeur.
        (ele)[0]->setNom("Daniel Wroughton Craig");
	// pour chacun des films : skylien et listeFilms[1], afficher titre et afficher nom du premier acteur.
        cout << "Nom du Film : " << ((listeFilms[0]))->getTitre() << ", premier acteur du Film :  " << ((ele)[0])->getNom() << " ";
        cout << "Nom du Film : " << ((listeFilms[1]))->getTitre() << ", premier acteur du Film :  " << ((elem)[0])->getNom();
        
        act.setElements(move(ele));
        skylien.setActeurs(move(act));
        acte.setElements(move(elem));
        listeFilms[1]->setActeurs(move(acte));
        //]
	
	// Détruit et enlève le premier film de la liste (Alien). 
	delete listeFilms[0];
         
        // afiche la nouvelle liste de films.
	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	for (int i = 0; i < listeFilms.getNElements(); ++i) {
	        cout << *(*((listeFilms.getElements()) + i)) << " ";
	}

	// Pour une couverture avec 0% de lignes non exécutées: 
	listeFilms.enleverFilm(nullptr); // Enlever un film qui n'est pas dans la liste (clairement que nullptr n'y est pas).
	
	Acteur* acteur = listeFilms.trouverActeur("N'existe pas");
	if  (acteur == nullptr) {
	        cout << "Aucun acteur de ce nom";
	} else { 
		cout << ligneDeSeparation << "Les films sont:";
	        // Affiche la liste des films. 
	        for (int i = 0; i < acteur->getJoueDans().getNElements(); ++i) {
		        cout << *(*((acteur->getJoueDans().getElements()) + i)) << " "; 
		}
	}
		
	//afficherFilmographieActeur(listeFilms, "N'existe pas"); // Afficher les films d'un acteur qui n'existe pas.

	// Détruire tout avant de terminer le programme. 
	listeFilms.detruire(true);
	return 0;
}