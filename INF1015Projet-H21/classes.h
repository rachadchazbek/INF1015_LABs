#include<iostream>
#include"Piece.cpp"

using namespace std;


class Piece
{
public:
    Piece(bool isWhite) : isWhite(isWhite) {}  // Constructeur de Piece.
    virtual ~Piece() {}                             // Destructeur de Piece.
    virtual char getPiece() = 0;                    // Getter de Piece, abstraite dans la classe Piece.
    bool getIsWhite() { return isWhite; } // Getter de la couleur de la Piece, cette mÈthode Ètant la mÍme dans toutes les classe qui dÈrivent de Piece, pas besoin qu'elle soit virtuel. Pas de besoin de l'overrride dans les classes enfantes. Retourne Vrai si la pece est white false si noire

private:
    bool isWhite;                              // Couleur de la Piece.
    virtual bool canMove(int ligneSource, int colSource, int ligneDestination, int colDestination, Case* echiquier[8][8]) = 0;  // La mÈthode canMove(), abstraite dans la classe Piece.
};

// Classe Roi qui hÈrite de Piece.
class Roi : public Piece
{
public:
    Roi(bool isWhite) : Piece(isWhite) {}                      // Constructeur de la classe Roi.
    ~Roi() {}                                                  // Destructeur de la classe Roi.
    char getPiece() { return 'R'; }                            // Getter de la Piece Roi.

private:
    bool canMove(int ligneSource, int colSource, int ligneDestination, int colDestination, Case* echiquier[8][8]);  // VÈrifie si le Roi peut move dans une case de l'Èchiquier, prend en paramËtres les coordonnÈes de dÈpart et les coordonnÈes d'arrivÈ, et l'Èchiquier.
};

// Classe Chevalier qui hÈrite de Piece.
class Chevalier : public Piece
{
public:
    Chevalier(char couleur) : Piece(couleur) {}               // Constructeur de la classe Chevalier.
    ~Chevalier() {}                                           // Destructeur de la classe Chevalier.
    char getPiece() { return 'C'; }                           // Getter de la Piece Chevalier.

private:
    bool canMove(int ligneSource, int colSource, int ligneDestination, int colDestination, Case* echiquier[8][8]);  // VÈrifie si le Chevalier peut move dans une case de l'Èchiquier, prend en paramËtres les coordonnÈes de dÈpart et les coordonnÈes d'arrivÈ, et l'Èchiquier.
};

// Classe Tour qui hÈrite de Piece.
class Tour : public Piece
{
public:
    Tour(char couleur) : Piece(couleur) {}                   // Constructeur de la Classe Tour.
    ~Tour() {}                                               // Destructeur de la classe Tour.
    char getPiece() { return 'T'; }                          // Getter de la Piece Tour.

private:
    bool canMove(int ligneSource, int colSource, int ligneDestination, int colDestination, Case* echiquier[8][8]);  // VÈrifie si la Tour peut move dans une case de l'Èchiquier, prend en paramËtres les coordonnÈes de dÈpart et les coordonnÈes d'arrivÈ, et l'Èchiquier.
};
