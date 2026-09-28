#ifndef EXPRESSIONLIB_H
#define EXPRESSIONLIB_H

#include <string>

// =====================================================================
//  Macro d'exportation / importation
// ---------------------------------------------------------------------
//  Lors de la compilation de la DLL, on definit EXPRESSIONLIB_EXPORTS :
//  les fonctions sont alors *exportees* (dllexport).
//  Lors de la compilation du programme qui l'utilise, la macro n'est pas
//  definie : les fonctions sont *importees* (dllimport).
//  Le meme fichier .h sert donc des deux cotes.
// =====================================================================

#if defined(_WIN32)
  #ifdef EXPRESSIONLIB_EXPORTS
    #define EXPRESSIONLIB_API __declspec(dllexport)
  #else
    #define EXPRESSIONLIB_API __declspec(dllimport)
  #endif
#else
  #define EXPRESSIONLIB_API          // sous Linux/macOS, rien n'est necessaire
#endif

// =====================================================================
//  L'interface publique de la bibliotheque : 3 fonctions, rien de plus.
//  La Pile generique et les fonctions utilitaires restent invisibles.
// =====================================================================

// Verifie parentheses et syntaxe. Remplit "raison" en cas de refus.
EXPRESSIONLIB_API bool estValide(const std::string& expression, std::string& raison);

// Indique si l'expression contient au moins une lettre (variable).
EXPRESSIONLIB_API bool contientVariable(const std::string& expression);

// Evalue une expression infixe. Leve une std::runtime_error en cas d'erreur.
EXPRESSIONLIB_API double evaluer(const std::string& expression);

#endif
