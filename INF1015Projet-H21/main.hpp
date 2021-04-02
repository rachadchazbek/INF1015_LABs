#include <iostream>
using namespace std;

class Piece;

class Case {
    public:
    Case(int x, int y) {
        this->x = x;
        this->y = y;
    }
    int getX() const {return x;}
    int getY() const {return y;}
    bool getIsOccupied() const {return isOccupied;}
    bool getIsOccupiedWhite() const {return isOccupiedWhite;}
    void setX(const int& s) {x = s;}
    void setY(const int& s) {y = s;}
    void setIsOccupied(const bool& b) {isOccupied = b;}
    void setIsOccupiedWhite(const bool& b) {isOccupiedWhite = b;}
    private:
    int x, y;
    bool isOccupied;
    bool isOccupiedWhite;
};
    

class Piece {
    public:
    Piece() {
        isAlive = 1;
        isWhite = 1;
        caseP = nullptr;
    }
    bool getIsAlive() const {return isAlive;}  // Getter isAlive.
    bool getIsWhite() const {return isWhite;}  // Getter isWhite.
    Case* getCase() {return caseP;}  // Getter case.
    void setIsAlive(const bool& b) {isAlive = b;}
    void setIsWhite(const bool& b) {isWhite = b;}
    void setCase(Case* c) {caseP = c;}
    
    protected:
    bool isAlive;
    bool isWhite;
    Case* caseP;
};


class Roi : public Piece{
    public:
    Roi(bool white) {
        isAlive = 1;
        isWhite = 1;
        if (white) {
            caseP = new Case(5, 0);
        }
        else{
            caseP = new Case(5,8);
        }
    }
    bool canMove(Case* caseM) {
        if(caseM->getX() > 8 || caseM->getX() < 0) {
            return 0;}
        
        if(caseM->getY() > 8 || caseM->getY() < 0) {
            return 0;}
        
        if(abs(caseM->getX() - caseP->getX()) > 1) {
            return 0;}
        
        if(abs(caseM->getY() - caseP->getY()) > 1) {
            return 0;}
        
        if(caseM->getIsOccupied()) {
            if (caseM->getIsOccupiedWhite()) {
                if (isWhite) {
                    return 0;
                }
                else {
                    return 1;
                }
            }
            else {
                if (isWhite) {
                    return 1;
                }
                else {
                    return 0;
                }
            }
        }
        
        return 1;
    }
};

