#ifndef PILE_TPP
#define PILE_TPP

#include "Pile.hpp"
#include <stdexcept>

template <typename T>
void Pile<T>::redimensionner() {
    capacite *= 2;
    T* nouveau = new T[capacite];
    for (int i = 0; i <= sommet; i++)
        nouveau[i] = elements[i];
    delete[] elements;
    elements = nouveau;
}

template <typename T>
Pile<T>::Pile(int capaciteInitiale): capacite(capaciteInitiale), sommet(-1) {
    elements = new T[capacite];
}
//(Check for potential caused errors by this destructor)
template <typename T>
Pile<T>::~Pile() {
    delete[] elements;
}

template <typename T>
void Pile<T>::empiler(const T& valeur) {
    if (sommet + 1 == capacite)
        redimensionner();
    elements[++sommet] = valeur;
}

template <typename T>
void Pile<T>::depiler() {
    if (estVide())
        throw std::underflow_error("Pile vide : impossible de dépiler");
    sommet--;
}

template <typename T>
T Pile<T>::sommetValeur() const {
    if (estVide())
        throw std::underflow_error("Pile vide : pas de sommet");
    return elements[sommet];
}

template <typename T>
bool Pile<T>::estVide() const {
    return sommet == -1;
}

template <typename T>
int Pile<T>::taille() const {
    return sommet + 1;
}

template <typename T>
int Pile<T>::getCapacite() const {
    return capacite;
}

#endif