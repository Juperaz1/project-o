#include "graphe.h"

// --- Ajout d’une tâche ---
void GrapheOrienté::ajouterTache(int id, const std::string& nom, int duree) {
    taches[id] = {nom, duree};
}

// --- Ajout d’un arc orienté ---
void GrapheOrienté::ajouterArc(int source, int destination) {
    taches[destination].dependances.push_back(source);
}

// --- Affichage simple du graphe ---
void GrapheOrienté::afficher() const {
    std::cout << "\n=== Graphe des dépendances ===\n";
    for (const auto& [id, t] : taches) {
        std::cout << "Tâche " << id << " (" << t.nom << ", " << t.duree << "j) dépend de : ";
        for (int d : t.dependances)
            std::cout << d << " ";
        std::cout << "\n";
    }
}

// --- Détection de cycle (DFS) ---
bool GrapheOrienté::dfsDetectCycle(int id, std::unordered_map<int, int>& etat) const {
    etat[id] = 1; // en cours
    for (int dep : taches.at(id).dependances) {
        if (etat[dep] == 1) return true; // cycle
        if (etat[dep] == 0 && dfsDetectCycle(dep, etat)) return true;
    }
    etat[id] = 2;
    return false;
}

bool GrapheOrienté::estRealisable() const {
    std::unordered_map<int, int> etat;
    for (const auto& [id, _] : taches) etat[id] = 0;
    for (const auto& [id, _] : taches)
        if (etat[id] == 0 && dfsDetectCycle(id, etat))
            return false;
    return true;
}

// --- Calcul du max des fins précédentes ---
int GrapheOrienté::maxFinPrecedentes(const std::vector<int>& deps) const {
    int max_fin = 0;
    for (int id : deps)
        max_fin = std::max(max_fin, taches.at(id).fin_tot);
    return max_fin;
}

// --- Calcul du min des débuts suivantes ---
int GrapheOrienté::minDebutSuivantes(int id) const {
    int min_debut = INT_MAX;
    for (const auto& [k, t] : taches)
        if (std::find(t.dependances.begin(), t.dependances.end(), id) != t.dependances.end())
            min_debut = std::min(min_debut, t.debut_tard);
    return (min_debut == INT_MAX) ? taches.at(id).fin_tot : min_debut;
}

// --- Calcul des dates au plus tôt / au plus tard / marges ---
void GrapheOrienté::calculerDates() {
    // Vérifie d’abord si le graphe est valide
    if (!estRealisable()) {
        std::cout << "⚠️  Projet non réalisable (cycle détecté)\n";
        return;
    }

    // Tri topologique simple (manuel ici)
    std::vector<int> ordre;
    for (const auto& [id, _] : taches) ordre.push_back(id);
    std::sort(ordre.begin(), ordre.end());

    // --- Calcul des dates au plus tôt ---
    for (int id : ordre) {
        Tache& t = taches[id];
        t.debut_tot = maxFinPrecedentes(t.dependances);
        t.fin_tot = t.debut_tot + t.duree;
    }

    // --- Fin du projet ---
    int fin_projet = 0;
    for (int id : ordre)
        fin_projet = std::max(fin_projet, taches[id].fin_tot);

    // --- Initialisation des dates au plus tard ---
    for (int id : ordre) {
        Tache& t = taches[id];
        t.fin_tard = fin_projet;
        t.debut_tard = fin_projet - t.duree;
    }

    // --- Calcul des dates au plus tard (retour en arrière) ---
    for (auto it = ordre.rbegin(); it != ordre.rend(); ++it) {
        int id = *it;
        Tache& t = taches[id];
        t.fin_tard = minDebutSuivantes(id);
        t.debut_tard = t.fin_tard - t.duree;
    }

    // --- Calcul des marges et tâches critiques ---
    for (auto& [id, t] : taches) {
        t.marge = t.debut_tard - t.debut_tot;
        t.critique = (t.marge == 0);
    }

    // --- Affichage ---
    std::cout << "\nID | Tâche | Début+Tôt | Début+Tard | Marge | Critique\n";
    std::cout << "-------------------------------------------------------\n";
    for (int id : ordre) {
        const Tache& t = taches.at(id);
        std::cout << id << " | " << t.nom;
        int spaces = std::max(0, 12 - (int)t.nom.size());
        std::cout << std::string(spaces, ' ')
                  << "| " << t.debut_tot
                  << " | " << t.debut_tard
                  << " | " << t.marge
                  << " | " << (t.critique ? "★" : "") << "\n";
    }

    std::cout << "-------------------------------------------------------\n";
    std::cout << "Durée minimale du projet : " << fin_projet << " jours\n";
}
