#include <iostream>
#include "classes.h"

using namespace std;

// Classe …chiquier.
class Echiquier
{
public:
    Echiquier();                  // Constructeur de la classe …chiquier.
    ~Echiquier();                 // Destructeur de la classe …chiquier.
    Case* echiquier[8][8];       // …chiquier qui est un tableau, 8 x 8, composÈ de pointeurs de Piece.
};

// Classe Case.
class Case {
public:

    Case(int x, int y) : x(x), y(y) {}
    ~Case() = default;
    Piece* getPiece() {return piece;}
    void setPiece(Piece* ppiece) {
        piece = ppiece;
    }
private:
    int x, y;
    Piece* piece;
}
