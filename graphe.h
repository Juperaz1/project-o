#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <fstream>
#include <sstream>

using namespace std;

<<<<<<< HEAD
struct Tache {
    string nom;
=======
struct Tache
{
    std::string nom;
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
    int duree;
    vector<int> dependances;
    int debut_tot = 0;
    int fin_tot = 0;
    int debut_tard = 0;
    int fin_tard = 0;
    int marge = 0;
    bool critique = false;
    int retardManuel = 0; // Retard imposé manuellement à la tâche
<<<<<<< HEAD
    int debut;       // calculé par l’algorithme
    int debutFixe = -1;  // -1 si aucune date fixe
    Tache(const string& n = "", int d = 0)
        : nom(n), duree(d), debut_tot(0), fin_tot(0),
          debut_tard(0), fin_tard(0), marge(0), critique(false),
          retardManuel(0), debutFixe(-1) {}


=======
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
};

// Classe représentant le graphe orienté des tâches du projet

class GrapheOrienté
{
private:
    // clé = ID de la tâche, valeur = structure Tache
    unordered_map<int, Tache> taches;

    // --- Méthodes internes ---
    bool DetectCycle(int id, unordered_map<int, int> &etat) const;
    int maxFinPrecedentes(const vector<int> &deps) const;
    int minDebutSuivantes(int id) const;

public:
    // --- Constructeurs / Destructeurs ---
    GrapheOrienté() = default;
    ~GrapheOrienté() = default;

    // --- Gestion du graphe ---
    void chargerDepuisFichier(const string &nomFichier);
    void sauvegarder(const std::string& nomFichier) const;
    void ajouterTache(int id, const string &nom, int duree);
    void ajouterArc(int source, int destination);
    void afficher() const;

    // --- Vérification de cohérence ---
    bool estRealisable() const;

    // --- Calculs d’ordonnancement ---
    void calculerDates();
    void modifierTache(int id, int nouvelleDuree, int decalageDebut);
    void modifierDebutTache(int id, int decalage);
<<<<<<< HEAD
    void setDebutFixe(int id, int dateFixe);
    
=======
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
};


#endif
