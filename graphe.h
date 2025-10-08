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
    string nom;
    int duree;
    vector<int> dependances;
    int debut_tot = 0;
    int fin_tot = 0;
    int debut_tard = 0;
    int fin_tard = 0;
    int marge = 0;
    bool critique = false;
    int retardManuel = 0; // Retard imposé manuellement à la tâche
    int debut;       // calculé par l’algorithme
    int debutFixe = -1;  // -1 si aucune date fixe
    Tache(const string& n = "", int d = 0)
        : nom(n), duree(d), debut_tot(0), fin_tot(0),
          debut_tard(0), fin_tard(0), marge(0), critique(false),
          retardManuel(0), debutFixe(-1) {}


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
    void modifierTache(int id, int nouvelleDuree, int decalageDebut);
    void modifierDebutTache(int id, int decalage);
    void setDebutFixe(int id, int dateFixe);
    
};


#endif
