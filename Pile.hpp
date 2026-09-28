#ifndef PILE_HPP
#define PILE_HPP

template <typename T>
class Pile {
private:
    T* elements;
    int capacite;
    int sommet;

    void redimensionner();

public:
    Pile(int capaciteInitiale = 10);
    ~Pile();

    void empiler(const T& valeur);
    void depiler();
    T sommetValeur() const;
    bool estVide() const;
    int taille() const ;
    int getCapacite() const;
};

#include "Pile.tpp"

#endif