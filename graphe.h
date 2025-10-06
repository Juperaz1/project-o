#ifndef GRAPHE_H
#define GRAPHE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stack>

struct Arc {
    int destination;
    int duree;
};

class GrapheOrienté {
private:
    std::unordered_map<int, std::vector<Arc>> adj;
    std::unordered_map<int, int> durees;

public:
    void ajouterTache(int id, int duree);
    void ajouterArc(int source, int destination);
    void afficher() const;
    bool estRealisable() const;

};

#endif