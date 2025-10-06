#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

struct Arc {
    int destination;
};

struct Tache {
    std::string nom;
    int duree;
    std::vector<int> dependances;
    int debut_tot = 0;
    int fin_tot = 0;
    int debut_tard = 0;
    int fin_tard = 0;
    int marge = 0;
    bool critique = false;
};

class GrapheOrienté {
private:
    std::unordered_map<int, Tache> taches;

    bool dfsDetectCycle(int id, std::unordered_map<int, int>& etat) const;
    int maxFinPrecedentes(const std::vector<int>& deps) const;
    int minDebutSuivantes(int id) const;

public:
    void ajouterTache(int id, const std::string& nom, int duree);
    void ajouterArc(int source, int destination);
    void afficher() const;
    bool estRealisable() const;
    void calculerDates();
};

#endif
