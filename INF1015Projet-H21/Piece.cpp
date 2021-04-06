#include <iostream>
#include "classes.h"


// Définition de la méthode canMove() de Roi.
bool Roi::canMove(int ligneSource, int colSource, int ligneDestination, int colDestination, Case* echiquier[8][8])
{
    if ((ligneDestination >= 0) and (ligneDestination <= 7) and (colDestination >= 0) and (colDestination <= 7)) {     // Vérifie si la destination est bien dans l'échiquier.
            Piece* destination = (echiquier[ligneDestination][colDestination])->getPiece();
            if ((destination == 0) or (this->getCouleurPiece() != destination->getCouleurPiece())) {                   // Vérifie si la case de destination est vide ou occupé par une pièce de couleur différente.
                if (((ligneDestination - ligneSource >= -1) and (ligneDestination - ligneSource <= 1)) and ((colDestination - colSource >= -1) and (colDestination - colSource <= 1))) {  // Vérifie si la case de destination est à une case de distance de la case de départ.
                    return true;
                }
            }
    }
    return false;
}

// Définition de la méthode canMove() de Chevalier.
bool Chevalier::canMove(int ligneSource, int colSource, int ligneDestination, int colDestination, Case* echiquier[8][8])
{
    if ((ligneDestination >= 0) and (ligneDestination <= 7) and (colDestination >= 0) and (colDestination <= 7)) {     // Vérifie si la destination est bien dans l'échiquier.
            Piece* destination = (echiquier[ligneDestination][colDestination])->getPiece();
            if ((destination == 0) or (this->getCouleurPiece() != destination->getCouleurPiece())) {                   // Vérifie si la case de destination est vide ou occupé par une pièce de couleur différente.
                if ((ligneSource == ligneDestination + 1) or (ligneSource == ligneDestination - 1)) {                  // Il existe 8, cases dans lesquels un chevalier peut se déplacer, les voicis.
                    if ((colSource == colDestination + 2) or (colSource == colDestination - 2)) {
                        return true;
                    }
                }
                if ((ligneSource == ligneDestination + 2) or (ligneSource == ligneDestination - 2)) {
                    if ((colSource == colDestination + 1) or (colSource == colDestination - 1)) {                          
                        return true;
                    }
                }
            }
    }
    return false;
}

// Définition de la méthode canMove() de Tour.
bool Tour::canMove(int ligneSource, int colSource, int ligneDestination, int colDestination, Case* echiquier[8][8])
{
    if ((ligneDestination >= 0) and (ligneDestination <= 7) and (colDestination >= 0) and (colDestination <= 7)) {     // Vérifie si la destination est bien dans l'échiquier.
            Piece* destination = (echiquier[ligneDestination][colDestination])->getPiece();
            if ((destination == 0) or (this->getCouleurPiece() != destination->getCouleurPiece())) {                   // Vérifie si la case de destination est vide ou occupé par une pièce de couleur différente.
                if (colSource == colDestination) {                                                                     // Vérifie si la Tour peut se déplacer verticalement.
                    int dclg;
                    if (ligneDestination - ligneSource > 0) {                                                          
                        dclg = 1;                                                                                      // Si ligneDestination - ligneSource > 0, la tour va monter à travers l'échiquier pour arriver à destination, il faudrait alors incrémenter ligneSource pour arriver à destination, le dclg devrait dans ce cas être égal à 1.
                    }
                    else {
                        dclg = -1;                                                                                     // Si ligneDestination - ligneSource < 0, la tour va descendre à travers l'échiquier pour arriver à destination, il faudrait alors décrémenter ligneSource pour arriver à destination, le dclg devrait dans ce cas être égal à -1.
                    }
                    for (int i = ligneSource + dclg; i != ligneDestination; i += dclg) {                               // Pour chaque ligne dans la colonne entre la case de départ et la case de destination, si une case != 0, en autre mots, si la case n'est pas vide, alors return false, si toutes les cases sont vides, return true.
                        if (echiquier[i][colSource] != 0) {
                            return false;
                        }
                    }
                    return true;
                }
                if (ligneSource == ligneDestination) {                                                                 // Vérifie si la Tour peut se déplacer horizontalement.
                    int dclg;
                    if (colDestination - colSource > 0) {
                        dclg = 1;                                                                                      // Si colDestination - colSource > 0, la tour va monter horizontalement à travers l'échiquier pour arriver à destination, il faudrait alors incrémenter colSource pour arriver à destination, le dclg devrait dans ce cas être égal à 1.
                    }
                    else {
                        dclg = -1;                                                                                    // Si ligneDestination - ligneSource < 0, la tour va descendre horizontalement à travers l'échiquier pour arriver à destination, il faudrait alors décrémenter ligneSource pour arriver à destination, le dclg devrait dans ce cas être égal à -1.
                    } 
                    for (int i = colSource + dclg; i != colDestination; i += dclg) {                                  // Pour chaque colonne dans la ligne entre la case de départ et la case de destination, si une case != 0, en autre mots, si la case n'est pas vide, alors return false, si toutes les cases sont vides, return true.
                        if (echiquier[ligneSource][i] != 0) {
                            return false;
                        }
                    }
                return true;
                }
            }
    }
    return false;
}

// Constructeur de la classe Échiquier.
Echiquier::Echiquier()
{
    for (int ligne = 0; ligne <= 7; ++ligne)
    {
        for (int col = 0; col <= 7; ++col) {  // Initialiser toutes les cases, avec 0, indiquant que ces cases sont vides.
            (echiquier[ligne][col])->setPiece(0);
        }
    }

    // Placer les Pieces de couleur Blanche.
    // Pour initialiser une partie, mettre tout les Pieces à leur place de départ.
    (echiquier[0][0])->setPiece(new Tour('B'));
    (echiquier[0][1])->setPiece(new Chevalier('B'));
    (echiquier[0][4])->setPiece(new Roi('B'));
    (echiquier[0][6])->setPiece(new Chevalier('B'));
    (echiquier[0][7])->setPiece(new Tour('B'));

    // Placer les Pieces de couleur Noir.
    (echiquier[7][0])->setPiece(new Tour('N'));
    (echiquier[7][1])->setPiece(new Chevalier('N'));
    (echiquier[7][4])->setPiece(new Roi('N'));
    (echiquier[7][6])->setPiece(new Chevalier('N'));
    (echiquier[7][7])->setPiece(new Tour('N'));
}

// Destructeur de la classe Échiquier.
Echiquier::~Echiquier()
{
    for (int ligne = 0; ligne <= 7; ++ligne)
    {
        for (int col = 0; col <= 7; ++col) {  // Delete tout les Pieces dans l'échiquier, et les remettre à 0.
            delete (echiquier[ligne][col])->getPiece();
            (echiquier[ligne][col])->setPiece(0);
            delete echiquier[ligne][col];
        }
    }
}

int main() {

    return 0;
}
