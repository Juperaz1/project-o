#include "graphe.h"
#include <iostream>

// --- Ajout d’une tâche ---
void GrapheOrienté::ajouterTache(int id, int duree) {
    durees[id] = duree;
    if (adj.find(id) == adj.end()) {
        adj[id] = {}; // initialise une liste vide d’arcs
    }
}

// --- Ajout d’un arc orienté ---
void GrapheOrienté::ajouterArc(int source, int destination) {
    if (durees.find(source) == durees.end() || durees.find(destination) == durees.end()) {
        std::cerr << "Erreur : tâche inconnue (" << source << " ou " << destination << ")\n";
        return;
    }
    adj[source].push_back({destination, durees[destination]});
}

// --- Affichage du graphe ---
void GrapheOrienté::afficher() const {
    std::cout << "\n=== Graphe orienté (tâches → dépendances) ===\n";
    for (const auto& [src, arcs] : adj) {
        std::cout << "Tâche " << src << " (" << durees.at(src) << "j) → ";
        for (const auto& arc : arcs) {
            std::cout << arc.destination << " (" << arc.duree << "j) ";
        }
        std::cout << "\n";
    }
}

// Fonction auxiliaire (DFS récursif)
bool dfsDetectCycle(int node,
                    std::unordered_map<int, int>& etat,
                    const std::unordered_map<int, std::vector<Arc>>& adj) {
    etat[node] = 1; // 1 = en cours de visite

    // Parcours des arcs sortants
    for (const auto& arc : adj.at(node)) {
        int voisin = arc.destination;

        if (etat[voisin] == 0) { // pas encore visité
            if (dfsDetectCycle(voisin, etat, adj)) return true;
        }
        else if (etat[voisin] == 1) {
            // Voisin encore en visite → cycle trouvé !
            return true;
        }
    }

    etat[node] = 2; // 2 = terminé
    return false;
}

bool GrapheOrienté::estRealisable() const {
    std::unordered_map<int, int> etat; // 0=non visité, 1=visite en cours, 2=fini

    // Initialiser tous les sommets à 0
    for (const auto& [id, _] : adj) {
        etat[id] = 0;
    }

    // Lancer un DFS depuis chaque sommet non visité
    for (const auto& [id, _] : adj) {
        if (etat[id] == 0) {
            if (dfsDetectCycle(id, etat, adj)) {
                std::cout << "⚠️  Cycle détecté : le projet n’est pas réalisable.\n";
                return false;
            }
        }
    }

    std::cout << "✅ Aucun cycle détecté : le projet est réalisable.\n";
    return true;
}
