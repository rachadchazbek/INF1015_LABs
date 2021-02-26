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
    T getElement(int index); // Returns an element. Should access the element with indedx in the shared pointer array.
    void addElement(T element, int index); // Add an element. Should take a shared pointer of the element and add it the array of shared pointer. Should also check for the array capacity. If is full should call doubleCapacity().
    void modifyElement(T element, int index); // Modify an element by changing the element that the pointers points at.

    Liste(const Liste &obj) { // Copie COnstructor, permits Liste<string> listeTextes2 = listeTextes;
        this->capacity = obj.capacity;
        this->nElements = obj.nElements;
        this->elements = make_unique<std::shared_ptr<T>[]>(capacity);
        for (int i = 0; i<obj.nElements; i++) {
            this->elements[i] = obj.elements[i];
        }
    }

private:
    int capacity, nElements; 
    unique_ptr<shared_ptr<T>[]> elements;// Unique pointer to an array of shared pointers. Each shared pointer, points to an Element.
    void doubleCapacity();// Should double the array of shared pointers capacity. Should be private.
};


template<typename T>  // Getter of element.
T Liste<T>::getElement(int index) {
    cout<< *elements[index] <<endl;
    return *elements[index];
}

template<typename T> // Adder of an element in the list elements. Creates a new shared ptr to the the element and moves it into the index of the old ptr
void Liste<T>::addElement(T element, int index) {
    auto ptr = make_shared<T>(element);
    elements[index] = move(ptr);
    nElements = nElements + 1;
}

template<typename T> // Modifier of an element in the list elements.
void Liste<T>::modifyElement(T element, int index) {
    *elements[index] = element;
}

struct Point { double x,y; };

int main() {
    // Implementation of the class Liste.
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


