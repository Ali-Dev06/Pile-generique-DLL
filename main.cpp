// =====================================================================
//  main.cpp : le programme client.
//  Il n'inclut QUE ExpressionLib.h : il ignore totalement l'existence
//  de la Pile generique, qui reste cachee dans la bibliotheque.
// =====================================================================

#include <iostream>
#include <string>
#include "ExpressionLib.h"
using namespace std;

void tester(const string& expression) {
    cout << "Expression : \"" << expression << "\"" << endl;

    string raison;
    if (!estValide(expression, raison)) {
        cout << "  Validation : INCORRECTE -> " << raison << endl << endl;
        return;
    }

    cout << "  Validation : CORRECTE" << endl;

    if (contientVariable(expression)) {
        cout << "  Evaluation : impossible (l'expression contient des variables)" << endl << endl;
        return;
    }

    try {
        double resultat = evaluer(expression);
        cout << "  Evaluation : " << resultat << endl;
    } catch (const exception& e) {
        cout << "  Evaluation : erreur -> " << e.what() << endl;
    }
    cout << endl;
}

void lancerExemples() {
    cout << endl << "----- Expressions correctes -----" << endl << endl;
    tester("((1+2)+5)*3");
    tester("(1  + 8      )*3");
    tester("[ (1+2) * {3+4} ] - 5");

    cout << "----- Expression avec variables -----" << endl << endl;
    tester("( x+y ) * z");

    cout << "----- Expressions incorrectes -----" << endl << endl;
    tester("((1+2)+5)*3}");
    tester("(5+5)*8/");
    tester("1++2");
}

void afficherMenu() {
    cout << "=========================================" << endl;
    cout << "   EVALUATEUR D'EXPRESSIONS (via la DLL)" << endl;
    cout << "=========================================" << endl;
    cout << "  1 - Tester ma propre expression" << endl;
    cout << "  2 - Lancer les exemples predefinis" << endl;
    cout << "  0 - Quitter" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Votre choix : ";
}

int main() {
    int choix = -1;
    string expression;

    while (choix != 0) {
        afficherMenu();

        if (!(cin >> choix)) {
            cin.clear();
            getline(cin, expression);
            cout << endl << "Choix invalide. Entrez 0, 1 ou 2." << endl << endl;
            continue;
        }
        getline(cin, expression);

        switch (choix) {
            case 1:
                cout << endl << "Entrez votre expression : ";
                getline(cin, expression);
                cout << endl;
                tester(expression);
                break;
            case 2:
                lancerExemples();
                break;
            case 0:
                cout << endl << "Fin du programme. Au revoir !" << endl;
                break;
            default:
                cout << endl << "Choix invalide. Entrez 0, 1 ou 2." << endl << endl;
                break;
        }
    }
    return 0;
}
