#include<iostream>
#include<string>
#include<memory>
//#include"structures.hpp"

using namespace std;

template<typename T>
class Liste {
public:
    Liste()  { // default Constructeur
        capacity = 100;
        nElements = 0;
        elements = make_unique<std::shared_ptr<T>[]>(capacity);
    }
    T getElement(int index); // Retourne un élément. Devrait accéder l'élément avec l'index de l'array des shared pointers.
    void addElement(T element, int index); // Ajouter un élément. Devrait prendre en parametre un shared pointer et le mettre dans l'array de shared pointers. Devrait aussi vérifier la capacité de l'array. Si l'array est remplit il faudrait call doubleCapacity().
    void modifyElement(T element, int index); // Modifie un élément.

    Liste(const Liste &obj) { // Copie Constructor, permits Liste<string> listeTextes2 = listeTextes;
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

int main() {
    // Implementation de la classe Liste.
    Liste<string> listeTextes;
    listeTextes.addElement("this is string 1", 0);
    listeTextes.addElement("this is string 2", 1);

    Liste<string> listeTextes2 = listeTextes;
    
    listeTextes.addElement("this is string 4", 0);
    listeTextes.modifyElement("this is string 3", 1);

    cout<< " ListeTextes[0] : ";
    listeTextes.getElement(0);
     cout<< " ListeTextes[1] : ";
    listeTextes.getElement(1);
     cout<< " ListeTextes2[0] : ";
    listeTextes2.getElement(0);
     cout<< " ListeTextes2[1] : ";
    listeTextes2.getElement(1);
    
    return 0;
}


