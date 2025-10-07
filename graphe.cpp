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
// --- Modifier une tâche et vérifier impact ---
void GrapheOrienté::modifierTache(int id, int nouvelleDuree, int decalageDuree = 0) {
    if (taches.find(id) == taches.end()) {
        std::cout << "⚠️  Tâche inexistante !\n";
        return;
    }

    Tache& t = taches[id];

    // On change seulement la durée si besoin
    t.duree = nouvelleDuree;

    // Recalcul des dates pour tout le projet
    calculerDates();

    // Vérifie si la tâche est critique après recalcul
    if (t.critique) {
        std::cout << "❌ Attention ! La tâche \"" << t.nom 
                  << "\" est sur le chemin critique, tout décalage retardera le projet !\n";
    } else {
        std::cout << "✅ Tâche modifiée : " << t.nom << "\n";
    }
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
t.debut_tot = std::max(maxFinPrecedentes(t.dependances), t.debut_tot) + t.retardManuel;
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
       int spaces = std::max(0, 12 - (int)t.nom.size());
       std::cout << std::string(spaces, ' ')
                 << "| " << t.debut_tot
                 << " | " << t.debut_tard
                 << " | " << t.marge
                 << " | " << (t.critique ? "★" : "") << "\n";
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
    //std::cout << "-------------------------------------------------------\n";
    //std::cout << "Durée minimale du projet : " << fin_projet << " jours\n";
}
void GrapheOrienté::modifierDebutTache(int id, int decalage) {
    if (taches.find(id) == taches.end()) {
        std::cout << "⚠️ Tâche inexistante !\n";
        return;
    }

    Tache& t = taches[id];

    // 1. Calcul de la durée actuelle du projet
    calculerDates();
    int ancienne_duree_projet = 0;
    for (const auto& [_, task] : taches)
        ancienne_duree_projet = std::max(ancienne_duree_projet, task.fin_tot);

    // 2. Appliquer le décalage manuel
    t.retardManuel += decalage;

    // 3. Recalcul des dates après décalage
    calculerDates();

    // 4. Nouvelle durée après recalcul
    int nouvelle_duree_projet = 0;
    for (const auto& [_, task] : taches)
        nouvelle_duree_projet = std::max(nouvelle_duree_projet, task.fin_tot);

    // 5. Affichage
    std::cout << "\n=== Résultat du décalage ===\n";
    std::cout << "Tâche \"" << t.nom << "\" (ID " << id << ") décalée de " << decalage << " jour(s)\n";
    std::cout << "Début au plus tôt : " << t.debut_tot << "\n";
    std::cout << "Début au plus tard : " << t.debut_tard << "\n";
    std::cout << "Marge restante : " << t.marge << " jour(s)\n";
    std::cout << "Chemin critique : " << (t.critique ? "★ Oui" : "Non") << "\n";

    if (nouvelle_duree_projet > ancienne_duree_projet) {
        std::cout << "❗ Impossible : la date minimale du projet est désormais retardée !\n";
        std::cout << "⏳ Le projet durera " << nouvelle_duree_projet
                  << " jours au lieu de " << ancienne_duree_projet << " jours.\n";
    } else if (nouvelle_duree_projet < ancienne_duree_projet) {
        std::cout << "✅ Le projet est raccourci : " 
                  << nouvelle_duree_projet << " jours au lieu de " << ancienne_duree_projet << ".\n";
    } else {
        std::cout << "✅ Ce décalage n’impacte pas la durée minimale du projet.\n";
    }

    std::cout << "============================\n";
}
