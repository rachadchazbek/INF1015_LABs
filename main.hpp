#include <iostream>
class Piece;

// Classe Case.
class Case 
{
public:
    Case(int positionX, int positionY) : x(positionX), y(positionY) {}  // Constructeur de la classe Case.
    Piece* getPiece() { return piece; }                                 // Getter de Piece.
    void setPiece(Piece* s) { piece = s; }                              // Setter de Piece.
    int getX() { return x; }                                            // Getter de la position X.
    int getY() { return y; }                                            // Getter de la position Y.
private:
    int x, y;
    Piece* piece;                                                       // Chaque case contient une Piece, si une case détient aucune Piece alors piece = 0.
};


// Classe Échiquier.
class Echiquier
{
public:
    Echiquier();                 // Constructeur de la classe Échiquier.
    ~Echiquier();                // Destructeur de la classe Échiquier.
    Case* echiquier[8][8];       // Échiquier qui est un tableau, 8 x 8, composé de pointeurs de Case.
};


// Classe Parente Piece.
class Piece
{
public:
    Piece(char couleur) : couleurPiece(couleur) {}  // Constructeur de Piece.
    virtual ~Piece() {}                             // Destructeur de Piece.
    virtual char getPiece() = 0;                    // Getter de Piece, abstraite dans la classe Piece.
    char getCouleurPiece() { return couleurPiece; } // Getter de la couleur de la Piece, cette méthode étant la même dans toutes les classe qui dérivent de Piece, pas besoin qu'elle soit virtuel. Pas de besoin de l'overrride dans les classes enfantes.
    void move(Case* caseDepart,Case* caseDestination,  Case* echiquier[8][8]);  // Fonction move, appliquable pour toutes les Pieces.
      
private:
    char couleurPiece;                              // Couleur de la Piece. Blanche ou Noir.
    virtual bool canMove(Case* caseDepart, Case* caseDestination,  Case* echiquier[8][8]) = 0;  // La méthode canMove(), abstraite dans la classe Piece.
};


// Classe Roi qui hérite de Piece.
class Roi : public Piece
{
public:
    Roi(char couleur) : Piece(couleur) {}                      // Constructeur de la classe Roi.
    ~Roi() {}                                                  // Destructeur de la classe Roi.
    char getPiece() { return 'R'; }                            // Getter de la Piece Roi.

private:
    bool canMove(Case* caseDepart, Case* caseDestination,  Case* echiquier[8][8]);  // Vérifie si le Roi peut move dans une case de l'échiquier, prend en paramètres les coordonnées de départ et les coordonnées d'arrivé, et l'échiquier.
};


// Classe Chevalier qui hérite de Piece.
class Chevalier : public Piece
{
public:
    Chevalier(char couleur) : Piece(couleur) {}               // Constructeur de la classe Chevalier.
    ~Chevalier() {}                                           // Destructeur de la classe Chevalier.
    char getPiece() { return 'C'; }                           // Getter de la Piece Chevalier.

private:
    bool canMove(Case* caseDepart, Case* caseDestination,  Case* echiquier[8][8]);  // Vérifie si le Chevalier peut move dans une case de l'échiquier, prend en paramètres les coordonnées de départ et les coordonnées d'arrivé, et l'échiquier.
};


// Classe Tour qui hérite de Piece.
class Tour : public Piece
{
public:
    Tour(char couleur) : Piece(couleur) {}                   // Constructeur de la Classe Tour.
    ~Tour() {}                                               // Destructeur de la classe Tour.
    char getPiece() { return 'T'; }                          // Getter de la Piece Tour.

private:
    bool canMove(Case* caseDepart, Case* caseDestination,  Case* echiquier[8][8]);  // Vérifie si la Tour peut move dans une case de l'échiquier, prend en paramètres les coordonnées de départ et les coordonnées d'arrivé, et l'échiquier.
};
