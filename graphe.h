#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using namespace std;

struct Tache {
    string nom;                // Nom de la tâche (ex: "Murs")
    int duree;                      // Durée de la tâche en jours
    vector<int> dependances;   // Liste des tâches dont celle-ci dépend
    int debut_tot = 0;              // Date de début au plus tôt
    int fin_tot = 0;                // Date de fin au plus tôt
    int debut_tard = 0;             // Date de début au plus tard
    int fin_tard = 0;               // Date de fin au plus tard
    int marge = 0;                  // Marge de flexibilité
    bool critique = false;          // Tâche critique (aucune marge)
};


// Classe représentant le graphe orienté des tâches du projet

class GrapheOrienté {
private:
    // clé = ID de la tâche, valeur = structure Tache
    unordered_map<int, Tache> taches;

    // --- Méthodes internes ---
    bool DetectCycle(int id, unordered_map<int, int>& etat) const;
    int maxFinPrecedentes(const vector<int>& deps) const;
    int minDebutSuivantes(int id) const;

public:
    // --- Constructeurs / Destructeurs ---
    GrapheOrienté() = default;
    ~GrapheOrienté() = default;

    // --- Gestion du graphe ---
    void ajouterTache(int id, const string& nom, int duree);
    void ajouterArc(int source, int destination);
    void afficher() const;

    // --- Vérification de cohérence ---
    bool estRealisable() const;

    // --- Calculs d’ordonnancement ---
    void calculerDates();
};

#endif
