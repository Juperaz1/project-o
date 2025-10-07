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

    for (const std::pair<const int, Tache>& element : taches) { 
        int id = element.first;
        const Tache& t = element.second;

        std::cout << "Tâche " << id << " (" << t.nom << ", " << t.duree << "j) dépend de : ";
        for (int d : t.dependances)
            std::cout << d << " ";
        std::cout << "\n";
    }
}

// --- Détection de cycle ---
bool GrapheOrienté::DetectCycle(int id, std::unordered_map<int, int>& etat) const {
    etat[id] = 1; // tâche en cours de visite

    const std::vector<int>& dependances = taches.at(id).dependances;
    for (int dep : dependances) { // Parcours des dépendances
        if (etat[dep] == 1) return true; // cycle détecté
        if (etat[dep] == 0 && DetectCycle(dep, etat)) return true; // appel récursif
    }

    etat[id] = 2; // tâche terminée
    return false;
}

bool GrapheOrienté::estRealisable() const {
    std::unordered_map<int, int> etat;

    for (const std::pair<const int, Tache>& element : taches) {
        int id = element.first;
        etat[id] = 0;
    }

    for (const std::pair<const int, Tache>& element : taches) {
        int id = element.first;
        if (etat[id] == 0 && DetectCycle(id, etat))
            return false;
    }

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

    for (const std::pair<const int, Tache>& element : taches) {
        const int k = element.first;
        const Tache& t = element.second;

        if (std::find(t.dependances.begin(), t.dependances.end(), id) != t.dependances.end()) {
            min_debut = std::min(min_debut, t.debut_tard);
        }
    }

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
    for (const std::pair<const int, Tache>& element : taches) {
        int id = element.first;
        ordre.push_back(id);
    }
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
    for (std::vector<int>::reverse_iterator it = ordre.rbegin(); it != ordre.rend(); ++it) {
        int id = *it;
        Tache& t = taches[id];
        t.fin_tard = minDebutSuivantes(id);
        t.debut_tard = t.fin_tard - t.duree;
    }

    // --- Calcul des marges et tâches critiques ---
    for (std::pair<const int, Tache>& element : taches) {
        int id = element.first;
        Tache& t = element.second;

        t.marge = t.debut_tard - t.debut_tot;
        t.critique = (t.marge == 0);
    }

    // --- Affichage ---
    std::cout << "\nID | Tâche | Début+Tôt | Début+Tard | Marge | Critique\n";
    std::cout << "-------------------------------------------------------\n";
    for (int id : ordre) {
        const Tache& t = taches.at(id);
        std::cout << id << " | " << t.nom;
        int spaces = std::max(0, 12 - static_cast<int>(t.nom.size()));
        std::cout << std::string(spaces, ' ')
                  << "| " << t.debut_tot
                  << " | " << t.debut_tard
                  << " | " << t.marge
                  << " | " << (t.critique ? "★" : "") << "\n";
    }

    std::cout << "-------------------------------------------------------\n";

    /* Chemin critique */
    std::vector<int> cheminCritique;
    for (std::pair<const int, Tache>& element : taches) {
        int id = element.first;
        Tache& t = element.second;

        t.marge = t.debut_tard - t.debut_tot;
        t.critique = (t.marge == 0);

        if (t.critique) {
            cheminCritique.push_back(id);
        }
    }

    std::reverse(cheminCritique.begin(), cheminCritique.end());
    std::cout << "Chemin critique : ";
    for (size_t i = 0; i < cheminCritique.size(); ++i) {
        if (i > 0) {
            std::cout << " -> ";
        }
        std::cout << cheminCritique[i];
    }
    std::cout << std::endl;

    /* Tâches pouvant être retardées */
    std::vector<int> margeTaches;
    for (std::pair<const int, Tache>& element : taches) {
        int id = element.first;
        Tache& t = element.second;

        if (t.debut_tard - t.debut_tot != 0) {
            margeTaches.push_back(id);
        }
    }

    std::reverse(margeTaches.begin(), margeTaches.end());
    std::cout << "Tâches avec marge : ";
    for (size_t i = 0; i < margeTaches.size(); ++i) {
        int id = margeTaches[i];
        Tache& t = taches[id];
        std::cout << id << "(" << t.marge << "j)";
        if (i < margeTaches.size() - 1) {
            std::cout << "; ";
        }
    }

    std::cout << std::endl;
    std::cout << "\nDurée minimale du projet : " << fin_projet << " jours\n";
}
