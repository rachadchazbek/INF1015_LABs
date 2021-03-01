
#include<iostream>

using namespace std;

struct ListeActeurs
{
};

class Item {
public:
  Item () {};

  friend ostream & operator<< (ostream & os, const Item & item){
    return item.print (os);
  }

  virtual ostream & print (ostream & os) const{
    os << "Annee : " << annee <<endl<< " Titre : " << titre << endl;
    return os;
  }
  
  void setAnnee (int annee){
    this->annee = annee;
  }
  
  void setTitre (string titre){
    this->titre = titre;
  }

protected:
  
  int annee;
  string titre;

};

//class Livre
class Livre:public Item {

public:
  Livre (string auteur, int nbCopieVendues, int nbPages, int annee,
	 string titre) {
    this->auteur = auteur;
    this->nbCopieVendues = nbCopieVendues;
    this->nbPages = nbPages;
    this->setTitre (titre);
    this->setAnnee (annee);
  };

  virtual ostream & print (ostream & os) const override{
    os <<"-->Film : " << titre<< endl << "Annee : "<< annee << endl << "Auteur : " << auteur << endl <<"Nombre de Copie Vendues : " << nbCopieVendues << endl <<
      "Nombres de Pages : " << nbPages << endl;
    return os;
  };


protected:
  string auteur;
  int nbCopieVendues, nbPages;
};

// classe Film
class Film:public Item {

public:Film (string realisateur, int recette, ListeActeurs acteurs,
	int annee, string titre) {

    this->realisateur = realisateur;
    this->recette = recette;
    this->acteurs = acteurs;
    this->setTitre (titre);
    this->setAnnee (annee);

  };

  virtual ostream & print (ostream & os) const override {

    os <<"-->Film : " << titre<< endl << "Annee : "<< annee << endl << "Realisateur: " << realisateur << endl <<"Recette : " << recette << endl <<
      "Acteurs : " << "acteurs" << endl;
    return os;
  };


  int getRecette () const { 

    return recette;
  }				// getter de la recette
  
  
  //void setActeurs(ListeActeurs s) {acteurs = move(s);} //setter de acteurs
  
 // ListeActeurs getActeurs() {return move(acteurs);} // getter de acteurs  


protected:
  string realisateur;		// Titre et nom du rC)alisateur (on suppose qu'il n'y a qu'un rC)alisateur).
  int recette;			// AnnC)e de sortie et recette globale du film en millions de dollars
  ListeActeurs acteurs;
};

int
main () {

  ListeActeurs acteurs;
  Livre livre1 ("bob", 21398, 509, 2001, "thee shadow");
  Film film1 ("steve", 800, acteurs, 2008, "batman begins");
  
  Film* p;
  p = &film1;
  
  cout<<*p<<endl;

  cout << livre1 << endl;
  cout << film1 << endl;
}
