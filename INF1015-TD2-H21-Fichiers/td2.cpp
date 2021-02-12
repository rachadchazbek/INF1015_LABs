#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include"ListeFilms.hpp"
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

//TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.

void ajouterFilm(ListeFilms liste1, Film film) {
	// mettre le pointeur du film dans le premier espace libre dans le tableau : si nullptr (espace libre) 
	if (liste1.capacite > liste1.nElements) {
		for (int i = 0; i < liste1.capacite; ++i) {
			if (*(liste1.elements + i) == nullptr) {
				*(liste1.elements + i) = &film;
				break;
			}
		}
		liste1.nElements = liste1.nElements + 1;
	} 
	else {
		// allocation d'un nouveau tableau qui est deux fois plus grand que l'ancien
		Film** new_arr = new Film* [liste1.capacite * 2];
		// copier le contenue de l'ancien tableau dans le nouveau tableau
		for (int i = 0; i < liste1.nElements; ++i) {
			new_arr[i] = *(liste1.elements + i);
		}
		new_arr[liste1.nElements] = &film;

		liste1.capacite = liste1.capacite * 2;
		liste1.nElements = liste1.nElements + 1;
		// suprimer l'ancien tableau et mettre nouveau tableau dans le pointeur de tableau
		delete[] liste1.elements;
		liste1.elements = new_arr;
	}
}


//TODO: Une fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.

	void enleverUnFilm(ListeFilms liste1, Film* pointeur_film) {
		// retrouver le pointeur vers le film, puis ensuite le remplacer par nullptr
		for (int i = 0; i < liste1.nElements; ++i) {
			if (*(liste1.elements + i) == pointeur_film) {
				*(liste1.elements + i) = nullptr;
				break;
			}
		}
		liste1.nElements -= 1;
	}


//TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.

	Acteur* trouverActeur(ListeFilms liste1, string acteur_nom) {
		// avec la liste de films, retrouver tout les films, puis retrouver tout les listes d'acteurs de ces films, si un de ces acteurs detient le nom "acteur_nom" retourner un pointeur vers lui
		for (int i = 0; i < liste1.nElements; ++i) {
			for (int j = 0; j < ((*(liste1.elements + i))->acteurs).nElements; ++j) {
				if (((*(((*(liste1.elements + i))->acteurs).elements + j))->nom) == acteur_nom) {
					return *(((*(liste1.elements + i))->acteurs).elements + j);
					break;
				}
				// si l'acteur existe pas dans la liste, retouner nullptr
				else {
					return nullptr;
				}
			}
		}
		return nullptr;
	}


// static potentiellement
//TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).
Acteur* lireActeur(istream& fichier, ListeFilms liste1)
{
	static Acteur acteur = {};
	acteur.nom            = lireString(fichier);
	acteur.anneeNaissance = lireUint16 (fichier);
	acteur.sexe           = lireUint8  (fichier);
	// si l'acteur existe dans la liste de films, retouner un pointeur vers cette acteur qui existe déja
	if (trouverActeur(liste1, acteur.nom) != nullptr) {
		return trouverActeur(liste1, acteur.nom);
	}
	// sinon retourne le nouveau pointeur vers l'acteur nouvellement crée
	else {
		return &acteur;
	}
	//TODO: Retourner un pointeur soit vers un acteur existant ou un nouvel acteur ayant les bonnes informations, selon si l'acteur existait déjà.  Pour fins de débogage, affichez les noms des acteurs crées; vous ne devriez pas voir le même nom d'acteur affiché deux fois pour la création.
}


Film* lireFilm(istream& fichier)
{
	static Film film = {};
	film.titre       = lireString(fichier);
	film.realisateur = lireString(fichier);
	film.anneeSortie = lireUint16 (fichier);
	film.recette     = lireUint16 (fichier);
	film.acteurs.nElements = lireUint8 (fichier);  //NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.  Vous pouvez aussi copier-coller les fonctions d'allocation de ListeFilms ci-dessus dans des nouvelles fonctions et faire un remplacement de Film par Acteur, pour réutiliser cette réallocation.
	ListeFilms liste;
	// allouer un nouveau tableau pour la liste d'acteurs
	film.acteurs.elements = new Acteur* [film.acteurs.nElements];
	// mettre acteur dans le premier espace vide du tableau
	for (int i = 0; i < film.acteurs.nElements; i++) {
		if (*(film.acteurs.elements + i) == nullptr) {
			*(film.acteurs.elements + i) = lireActeur(fichier, liste); //TODO: Placer l'acteur au bon endroit dans les acteurs du film.
			film.acteurs.nElements += 1;
			//TODO: Ajouter le film à la liste des films dans lesquels l'acteur joue.
			ajouterFilm((*(film.acteurs.elements + i))->joueDans, film);
		}
	}
	// retouner pointeur vers ce film
	return &film; //TODO: Retourner le pointeur vers le nouveau film.
}


ListeFilms creerListe(string nomFichier)
{
	static ListeFilms liste = {};
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	
	int liste.nElements = lireUint16(fichier);

	//TODO: Créer une liste de films vide.
	liste.elements = new Film* [liste.nElements];
    // pour le premier espace du nouveau tableau, placer lireFilm(fichier)
	for (int i = 0; i < liste.nElements; i++) {
		if (*(liste.elements + i) == nullptr) {
			*(liste.elements + i) = lireFilm(fichier); //TODO: Ajouter le film à la liste.
		}
	}
	// retouner la liste de films
	return (liste); //TODO: Retourner la liste de films.
}


//TODO: Une fonction pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Noter qu'il faut enleve le film détruit des films dans lesquels jouent les acteurs.  Pour fins de débogage, affichez les noms des acteurs lors de leur destruction.
void detruireFilm(Film film) {

	cout << "Destruction du film : " << film.titre << "\n";
	// detruire le film des listes dans lequels joue les acteurs joues (delete le pointeur du film de la liste joueDans des acteurs du film)
	for (int i = 0; i < (film.acteurs).capacite; ++i) {
		for (int j = 0; j < ((*((film.acteurs).elements + i))->joueDans).capacite; ++j){
			if (*(((*((film.acteurs).elements + i))->joueDans).elements + j) == &film) {

				*(((*((film.acteurs).elements + i))->joueDans).elements + j) = nullptr;
				((*((film.acteurs).elements + i))->joueDans).nElements -= 1;
			}
		}	
	}
	// si un acteur ne joue plus dans aucun film, detruire sa mémoire, donc detruire la liste joueDans de l'acteur
	for (int i = 0; i < (film.acteurs).capacite; ++i) {
		if (((*((film.acteurs).elements + i))->joueDans).nElements == 0) {

			delete[]  ((*((film.acteurs).elements + i))->joueDans).elements;
		}
	}
	// delete la liste des acteurs du film
	delete [] (film.acteurs).elements;
}


//TODO: Une fonction pour détruire une ListeFilms et tous les films qu'elle contient.
void detruireListeDeFilms(ListeFilms liste1) {
	// detruire chaques film de la Liste de Films
	for (int i = 0; i < liste1.nElements; ++i) {
		detruireFilm(*(*(liste1.elements + i)));
	}
	// detruire la Liste de Films
	delete[] liste1.elements;
}


void afficherActeur(const Acteur& acteur)
{
	cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}


//TODO: Une fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).
void afficherActeurs(Film film) {
	// afficher chaque acteur contenue dans la liste des acteurs d'un film (Liste Acteur)
	cout << "Le nom du Film : " << film.titre << "\n";
	for (int i = 0; i < (film.acteurs).nElements; ++i) {
		afficherActeur(*(*((film.acteurs).elements + i)));
	}
}


void afficherListeFilms(const ListeFilms& listeFilms)
{
	//TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
	static const string ligneDeSeparation = "\n";
	cout << ligneDeSeparation;
	//TODO: Changer le for pour utiliser un span.
	// afficher le titre de chaque film dans une Liste de Films
	for (int i = 0; i < listeFilms.nElements; i++) {
		cout << (*(listeFilms.elements + i))->titre;
		cout << ligneDeSeparation;
	}
}


void afficherFilmographieActeur(const ListeFilms& listeFilms, const string& nomActeur)
{
	//TODO: Utiliser votre fonction pour trouver l'acteur (au lieu de le mettre à nullptr).
	// on trouve l'acteur avec une listeFilms
	const Acteur* acteur = trouverActeur(listeFilms, nomActeur);
	// si on trouve pas l'acteur en question
	if (acteur == nullptr)
		cout << "Aucun acteur de ce nom" << endl;
	else
		afficherListeFilms(acteur->joueDans);
}

int main()
{
#ifdef VERIFICATION_ALLOCATION_INCLUS
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
#endif
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	int* fuite = new int; //TODO: Enlever cette ligne après avoir vérifié qu'il y a bien un "Detected memory leak" de "4 bytes" affiché dans la "Sortie", qui réfère à cette ligne du programme.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	//TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lignes, en appelant les fonctions écrites.

	//TODO: La ligne suivante devrait lire le fichier binaire en allouant la mémoire nécessaire.  Devrait afficher les noms de 20 acteurs sans doublons (par l'affichage pour fins de débogage dans votre fonction lireActeur).
	ListeFilms listeFilms = creerListe("films.bin");
	// afficher chaque acteur dans listeFilms
	for (int i = 0; i < listeFilms.nElements; ++i) {
		afficherActeurs(*(*(listeFilms.elements) + i));
	}

	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	//TODO: Afficher le premier film de la liste.  Devrait être Alien.
	cout << ((*(listeFilms.elements))->titre);

	cout << ligneDeSeparation << "Les films sont:" << endl;
	//TODO: Afficher la liste des films.  Il devrait y en avoir 7.
	afficherListeFilms(listeFilms);

	//TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.
	// changer la date de naissance de Benedict Cumberbatch en le trouvant avec : trouverActeur
	((trouverActeur(listeFilms, "Benedict Cumberbatch"))->anneeNaissance) = 1976;

	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	//TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
	afficherListeFilms(((trouverActeur(listeFilms, "Benedict Cumberbatch")->joueDans)));

	//TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.
	detruireFilm(*(*(listeFilms.elements)));

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	//TODO: Afficher la liste des films.
	afficherListeFilms(listeFilms);

	//TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.

	//TODO: Détruire tout avant de terminer le programme.  L'objet verifierFuitesAllocations devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
	detruireListeDeFilms(listeFilms);

	return 0;
}