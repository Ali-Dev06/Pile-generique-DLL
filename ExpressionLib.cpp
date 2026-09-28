// =====================================================================
//  ExpressionLib.cpp : le corps de la bibliotheque dynamique.
//  Compile une seule fois dans ExpressionLib.dll.
//
//  Tout ce qui est declare "static" ici a une portee interne au fichier :
//  ces fonctions existent dans la DLL mais ne sont pas exportees, donc
//  invisibles depuis le programme appelant. Seules les 3 fonctions
//  declarees dans ExpressionLib.h franchissent la frontiere de la DLL.
// =====================================================================

#include "ExpressionLib.h"
#include "Pile.hpp"          // la pile generique : cachee dans la DLL
#include <stdexcept>

// ---------------------------------------------------------------------
//  Outils internes (non exportes)
// ---------------------------------------------------------------------

static bool estEspace(char c)  { return c == ' ' || c == '\t'; }
static bool estChiffre(char c) { return c >= '0' && c <= '9'; }
static bool estLettre(char c)  { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
static bool estOuvrante(char c){ return c == '(' || c == '[' || c == '{'; }
static bool estFermante(char c){ return c == ')' || c == ']' || c == '}'; }
static bool estOperateur(char c){ return c == '+' || c == '-' || c == '*' || c == '/'; }

static bool correspond(char ouvrante, char fermante) {
    return (ouvrante == '(' && fermante == ')')
        || (ouvrante == '[' && fermante == ']')
        || (ouvrante == '{' && fermante == '}');
}

static int priorite(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;                        // une parenthese ouvrante
}

// Depile un operateur et ses deux operandes, calcule, et rempile le resultat
static void appliquerSommet(Pile<double>& operandes, Pile<char>& operateurs) {
    char op = operateurs.sommetValeur(); operateurs.depiler();
    if (operandes.taille() < 2) throw std::runtime_error("Expression mal formee");

    double b = operandes.sommetValeur(); operandes.depiler();
    double a = operandes.sommetValeur(); operandes.depiler();

    double resultat;
    if      (op == '+') resultat = a + b;
    else if (op == '-') resultat = a - b;
    else if (op == '*') resultat = a * b;
    else if (b != 0)    resultat = a / b;
    else throw std::runtime_error("Division par zero");

    operandes.empiler(resultat);
}

// ---------------------------------------------------------------------
//  Fonctions exportees
// ---------------------------------------------------------------------

EXPRESSIONLIB_API bool estValide(const std::string& expression, std::string& raison) {
    Pile<char> pile;
    bool attendOperande = true;   // au debut, on attend forcement un operande
    bool vide = true;

    size_t i = 0;
    while (i < expression.size()) {
        char c = expression[i];
        if (estEspace(c)) { i++; continue; }      // les espaces sont ignores
        vide = false;

        // --- un operande : nombre ou variable ---
        if (estChiffre(c) || estLettre(c)) {
            if (!attendOperande) { raison = "deux operandes se suivent sans operateur"; return false; }
            while (i < expression.size() && (estChiffre(expression[i]) || estLettre(expression[i])))
                i++;
            attendOperande = false;               // on attend maintenant un operateur
            continue;
        }

        // --- parenthese ouvrante : empilee, on attend toujours un operande ---
        if (estOuvrante(c)) {
            if (!attendOperande) { raison = "operateur manquant avant une parenthese ouvrante"; return false; }
            pile.empiler(c); i++; continue;
        }

        // --- parenthese fermante : doit correspondre au sommet de la pile ---
        if (estFermante(c)) {
            if (attendOperande) { raison = "operande manquant avant une parenthese fermante"; return false; }
            if (pile.estVide())  { raison = "parenthese fermante sans ouvrante correspondante"; return false; }
            char ouvrante = pile.sommetValeur();
            pile.depiler();
            if (!correspond(ouvrante, c)) { raison = "mauvaise correspondance de parentheses"; return false; }
            i++; continue;
        }

        // --- operateur : exige un operande apres lui ---
        if (estOperateur(c)) {
            if (attendOperande) { raison = "operande manquant avant l'operateur"; return false; }
            attendOperande = true; i++; continue;
        }

        raison = "caractere non reconnu"; return false;
    }

    // --- controles de fin ---
    if (vide)            { raison = "expression vide"; return false; }
    if (attendOperande)  { raison = "operande manquant a la fin de l'expression"; return false; }
    if (!pile.estVide()) { raison = "parenthese ouvrante jamais fermee"; return false; }

    raison = ""; return true;
}

EXPRESSIONLIB_API bool contientVariable(const std::string& expression) {
    for (size_t i = 0; i < expression.size(); i++)
        if (estLettre(expression[i])) return true;
    return false;
}

EXPRESSIONLIB_API double evaluer(const std::string& expression) {
    Pile<double> operandes;     // les valeurs et les resultats intermediaires
    Pile<char>   operateurs;    // les operateurs en attente et les ouvrantes

    size_t i = 0;
    while (i < expression.size()) {
        char c = expression[i];
        if (estEspace(c)) { i++; continue; }

        if (estChiffre(c)) {                  // lecture d'un nombre entier
            double nombre = 0;
            while (i < expression.size() && estChiffre(expression[i])) {
                nombre = nombre * 10 + (expression[i] - '0'); i++;
            }
            operandes.empiler(nombre);
            continue;
        }

        if (estOuvrante(c)) { operateurs.empiler(c); i++; continue; }

        if (estFermante(c)) {                 // on vide jusqu'a l'ouvrante
            while (!operateurs.estVide() && !estOuvrante(operateurs.sommetValeur()))
                appliquerSommet(operandes, operateurs);
            if (operateurs.estVide()) throw std::runtime_error("Parenthese fermante en trop");
            operateurs.depiler();             // retire l'ouvrante
            i++; continue;
        }

        if (estOperateur(c)) {                // on applique d'abord les priorites >=
            while (!operateurs.estVide()
                   && !estOuvrante(operateurs.sommetValeur())
                   && priorite(operateurs.sommetValeur()) >= priorite(c))
                appliquerSommet(operandes, operateurs);
            operateurs.empiler(c);
            i++; continue;
        }

        throw std::runtime_error("Caractere non reconnu");
    }

    while (!operateurs.estVide())             // les operateurs restants
        appliquerSommet(operandes, operateurs);

    if (operandes.taille() != 1) throw std::runtime_error("Expression mal formee");
    return operandes.sommetValeur();
}
