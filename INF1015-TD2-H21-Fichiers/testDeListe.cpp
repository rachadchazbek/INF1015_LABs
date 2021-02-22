#include<iostream>
#include<string>
#include<memory>
//#include"structures.hpp"
using namespace std;

template<typename T>
class Liste {
public:
    Liste(); //default Constructeur 
    T getElement(int index); // Returns an element. Should access the element with indedx in the shared pointer array 
    void addElement(T element, int index); //add an element. Should take a shared pointer of the element and add it the array of shared pointer. Should also check for the array capacity. If is full should call doubleCapacity().
    void modifyElement(T element, int index);

    Liste operator=(Liste &Liste2) {
        capacity = Liste2.capacity;
        nElements = Liste2.elements;
        elements = Liste2.elements;
        return *this;
    }

private:
    int capacity, nElements;
    unique_ptr<T[]> elements;// Unique pointer to an array of shared pointers. Each shared pointer, points to an Element
    void doubleCapacity();// should double the array of shared pointers capacity. Should be private.
};

template<typename T>
Liste<T>::Liste() {
    capacity = 100;
    nElements = 0;
    elements = unique_ptr<T[]>(new T[capacity]);
}

template<typename T> 
T Liste<T>::getElement(int index) {
    cout<< this->elements[index] <<endl;
    return elements[index];
}

template<typename T>
void Liste<T>::addElement(T element, int index) {
    elements[index] = make_shared<T>(element);
    nElements = nElements + 1;
}

template<typename T>
void Liste<T>::modifyElement(T element, int index) {
    auto ptr = make_shared<T>(element);
    elements[index] = std::move(ptr);
}

struct Point { double x,y; };

int main() {
    Liste<string> listeTextes;
    listeTextes.addElement("this is string1", 0);
    listeTextes.addElement("this is string2", 1);

    listeTextes2 = listeTextes;
    listeTextes2.modifyElement("this is string1", 0);
    listeTextes2.modifyElement("this is string3", 1);

    cout<< " ListeTextes[0] : "<< endl;
    listeTextes.getElement(0);
     cout<< " ListeTextes[1] : "<< endl;
    listeTextes.getElement(1);
     cout<< " ListeTextes2[0] : "<< endl;
    listeTextes2.getElement(0);
     cout<< " ListeTextes2[1] : "<< endl;
    listeTextes2.getelement(1);
    return 0;
}

