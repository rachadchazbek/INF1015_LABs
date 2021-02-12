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

// le constructeur de notre classe
ListeFilms::ListeFilms(int capa, int nElem, Film** elem) {
	capacite = capa;
	nElements = nElem;
	elements = elem;
}


void ListeFilms::ajouterFilm(Film film) {

	if (capacite > nElements) {
		// mettre le pointeur du film dans le premier espace libre dans le tableau : si nullptr (espace libre) 
		for (int i = 0; i < capacite; ++i) {
			if (*(elements + i) == nullptr) {
				*(elements + i) = &film;
				break;
			}
		}
		nElements += 1;
	}
	else {
		// allocation d'un nouveau tableau
		Film** new_arr = new Film* [capacite * 2];
		// recopier tout les elements de l'ancien tableau dans le nouveau
		for (int i = 0; i < nElements; ++i) {
			new_arr[i] = *(elements + i);
		}
		new_arr[nElements] = &film;

		capacite = capacite * 2;
		nElements = nElements + 1;
		// suprimer l'ancien tableau et mettre le nouveau tableau a sa place
		delete[] elements;
		elements = new_arr;
	}

}


void ListeFilms::enleverUnFilm(Film* pointeur_film) {
	// retrouver le pointeur vers le film, puis ensuite le remplacer par nullptr
	for (int i = 0; i < nElements; ++i) {
		if (*(elements + i) == pointeur_film) {
			*(elements + i) = nullptr;
			break;
		}
	}
	nElements -= 1;
}


Acteur* ListeFilms::trouverActeur(string acteur_nom) const {
	// avec la liste de films, retrouver tout les films, puis retrouver tout les listes d'acteurs de ces films, si un de ces acteurs detient le nom "acteur_nom" retourner un pointeur vers lui
	for (int i = 0; i < nElements; ++i) {
		for (int j = 0; j < ((*(elements + i))->acteurs).nElements; ++j) {
			if (((*(((*(elements + i))->acteurs).elements + j))->nom) == acteur_nom) {
				return *(((*(elements + i))->acteurs).elements + j);
				break;
			}
			else {
				return nullptr;
			}
		}
	}
	return nullptr;
}


void ListeFilms::detruireFilm(Film film) {

	cout << "Destruction du film : " << film.titre << "\n";
	// detruire le film des listes dans lequels joue les acteurs joues (delete le pointeur du film de la liste joueDans des acteurs du film)
	for (int i = 0; i < (film.acteurs).capacite; ++i) {
		for (int j = 0; j < (((*((film.acteurs).elements + i))->joueDans).capacite); ++j) {
			if (*(((*((film.acteurs).elements + i))->joueDans).elements + j) == &film) {

				*(((*((film.acteurs).elements + i))->joueDans).elements + j) = nullptr;
				((*((film.acteurs).elements + i))->joueDans).nElements -= 1;
			}
		}
	}
	// si un acteur ne joue plus dans aucun film, detruire sa mémoire, donc detruire la liste joueDans de l'acteur
	for (int i = 0; i < (film.acteurs).capacite; ++i) {
		if (((*((film.acteurs).elements + i))->joueDans).nElements == 0) {

			delete[]((*((film.acteurs).elements + i))->joueDans).elements;
		}
	}
	// delete la liste des acteurs du film
	delete[](film.acteurs).elements;
}


void ListeFilms::detruireListeDeFilms() {
	// detruire chaque films dans ListeFilms
	for (int i = 0; i < nElements; ++i) {
		detruireFilm(*(*(elements + i)));
	}
	// delete la ListeFilms
	delete[] elements;
}


void ListeFilms::afficherActeur(const Acteur& acteur) const
{
	cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}


void ListeFilms::afficherActeurs(Film film) const {
	cout << "Le nom du Film : " << film.titre << "\n";
	// afficher chaque acteurs dans le film (Liste Acteurs)
	for (int i = 0; i < (film.acteurs).nElements; ++i) {
		afficherActeur(*(*((film.acteurs).elements + i)));
	}
}


void ListeFilms::afficherListeFilms() const
{
	//TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
	static const string ligneDeSeparation = "\n";
	cout << ligneDeSeparation;
	//TODO: Changer le for pour utiliser un span.
	// afficher chaque film dans la ListeFilms
	for (int i = 0; i < nElements; i++) {
		cout << (*(elements + i))->titre;
		cout << ligneDeSeparation;
	}
}


void ListeFilms::afficherFilmographieActeur(const string& nomActeur) const
{
	//TODO: Utiliser votre fonction pour trouver l'acteur (au lieu de le mettre à nullptr).
	const Acteur* acteur = trouverActeur(nomActeur);
	// si on retrouve pas l'acteur
	if (acteur == nullptr)
		cout << "Aucun acteur de ce nom" << endl;
	else
		afficherListeFilms();
}

