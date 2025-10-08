#include "graphe.h"

// --- Ajout d’une tâche ---
void GrapheOrienté::ajouterTache(int id, const std::string& nom, int duree) {
    Tache t;
    t.nom = nom;
    t.duree = duree;
    t.debut_tot = 0;
    t.fin_tot = 0;
    t.debut_tard = 0;
    t.fin_tard = 0;
    t.marge = 0;
    t.critique = false;
    t.retardManuel = 0;
    t.debutFixe = -1;

    taches[id] = t;
}

// --- Ajout d’un arc orienté ---
void GrapheOrienté::ajouterArc(int source, int destination) {
    taches[destination].dependances.push_back(source);
}

// --- Affichage simple du graphe ---
void GrapheOrienté::afficher() const {
    std::cout << "\n=== Graphe des dépendances ===\n";

    for (const auto& element : taches) { 
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
    etat[id] = 1;

    for (int dep : taches.at(id).dependances) {
        if (etat[dep] == 1) return true;
        if (etat[dep] == 0 && DetectCycle(dep, etat)) return true;
    }

    etat[id] = 2;
    return false;
}

bool GrapheOrienté::estRealisable() const {
    std::unordered_map<int, int> etat;

    for (const auto& element : taches)
        etat[element.first] = 0;

    for (const auto& element : taches)
        if (etat[element.first] == 0 && DetectCycle(element.first, etat))
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

    for (const auto& element : taches) {
        const Tache& t = element.second;

        if (std::find(t.dependances.begin(), t.dependances.end(), id) != t.dependances.end()) {
            min_debut = std::min(min_debut, t.debut_tard);
        }
    }

    return (min_debut == INT_MAX) ? taches.at(id).fin_tot : min_debut;
}

// --- Modifier une tâche et vérifier impact ---
void GrapheOrienté::modifierTache(int id, int nouvelleDuree, int decalageDuree) {
    if (taches.find(id) == taches.end()) {
        std::cout << "⚠️  Tâche inexistante !\n";
        return;
    }

    Tache& t = taches[id];
    t.duree = nouvelleDuree;

    calculerDates();

    if (t.critique) {
        std::cout << "❌ Attention ! La tâche \"" << t.nom 
                  << "\" est sur le chemin critique, tout décalage retardera le projet !\n";
    } else {
        std::cout << "✅ Tâche modifiée : " << t.nom << "\n";
    }
}

// --- Calcul des dates au plus tôt / au plus tard / marges ---
void GrapheOrienté::calculerDates() {
    if (!estRealisable()) {
        std::cout << "⚠️  Projet non réalisable (cycle détecté)\n";
        return;
    }

    std::vector<int> ordre;
    for (const auto& element : taches)
        ordre.push_back(element.first);
    std::sort(ordre.begin(), ordre.end());

    // --- Étape 1 : Calcul des dates au plus tôt ---
    for (int id : ordre) {
        Tache& t = taches[id];

        if (t.debutFixe != -1)
            t.debut_tot = t.debutFixe + t.retardManuel;
        else
            t.debut_tot = std::max(maxFinPrecedentes(t.dependances), t.debut_tot) + t.retardManuel;

        t.fin_tot = t.debut_tot + t.duree;
    }

    // --- Étape 2 : Forcer chevauchement T3 & T4 >= 1 unité ---
    if (taches.count(3) && taches.count(4)) {
        Tache& t3 = taches[3];
        Tache& t4 = taches[4];

        if (t3.fin_tot <= t4.debut_tot) {
            t3.debut_tot = t4.debut_tot; // commence en même temps que T4
            t3.fin_tot = t3.debut_tot + t3.duree;
        }
    }

    // --- Étape 3 : T5 ne peut commencer avant le début de T3 ---
    if (taches.count(3) && taches.count(5)) {
        Tache& t3 = taches[3];
        Tache& t5 = taches[5];

        if (t5.debut_tot < t3.debut_tot) {
            t5.debut_tot = t3.debut_tot;
            t5.fin_tot = t5.debut_tot + t5.duree;
        }
    }

    // --- Étape 4 : Fin projet ---
    int fin_projet = 0;
    for (int id : ordre)
        fin_projet = std::max(fin_projet, taches[id].fin_tot);

    // --- Étape 5 : Initialiser au plus tard à la fin du projet ---
    for (int id : ordre) {
        Tache& t = taches[id];
        t.fin_tard = fin_projet;
        t.debut_tard = fin_projet - t.duree;
    }

    // --- Étape 6 : Calcul des dates au plus tard (retour) ---
    for (auto it = ordre.rbegin(); it != ordre.rend(); ++it) {
        int id = *it;
        Tache& t = taches[id];
        t.fin_tard = minDebutSuivantes(id);
        t.debut_tard = t.fin_tard - t.duree;
    }

    // --- Étape 7 : Marges et critiques ---
    for (auto& [_, t] : taches) {
        t.marge = t.debut_tard - t.debut_tot;
        t.critique = (t.marge == 0);
    }

    // --- Affichage ---
    std::cout << "\nID | Tâche | Début+Tôt | Début+Tard | Marge | Critique\n";
    std::cout << "-------------------------------------------------------\n";
    for (int id : ordre) {
        const Tache& t = taches.at(id);
        std::cout << id << " | " << t.nom
                  << std::string(std::max(0, 12 - (int)t.nom.size()), ' ')
                  << "| " << t.debut_tot
                  << " | " << t.debut_tard
                  << " | " << t.marge
                  << " | " << (t.critique ? "★" : "") << "\n";
    }

    // --- Chemin critique ---
    std::vector<int> cheminCritique;
    for (const auto& [id, t] : taches)
        if (t.critique) cheminCritique.push_back(id);

    std::reverse(cheminCritique.begin(), cheminCritique.end());
    std::cout << "Chemin critique : ";
    for (size_t i = 0; i < cheminCritique.size(); ++i)
        std::cout << (i > 0 ? " -> " : "") << cheminCritique[i];
    std::cout << "\n";

    // --- Tâches avec marge ---
    std::vector<int> margeTaches;
    for (const auto& [id, t] : taches)
        if (t.marge != 0) margeTaches.push_back(id);

    std::reverse(margeTaches.begin(), margeTaches.end());
    std::cout << "Tâches avec marge : ";
    for (size_t i = 0; i < margeTaches.size(); ++i) {
        int id = margeTaches[i];
        const Tache& t = taches.at(id);
        std::cout << id << "(" << t.marge << "j)";
        if (i < margeTaches.size() - 1) std::cout << "; ";
    }
    std::cout << "\n";

    std::cout << "\nDurée minimale du projet : " << fin_projet << " jours\n";
}


// --- Décaler une tâche ---
void GrapheOrienté::modifierDebutTache(int id, int decalage) {
    if (taches.find(id) == taches.end()) {
        std::cout << "⚠️ Tâche inexistante !\n";
        return;
    }

    Tache& t = taches[id];

    calculerDates();
    int ancienne_duree_projet = 0;
    for (const auto& [_, task] : taches)
        ancienne_duree_projet = std::max(ancienne_duree_projet, task.fin_tot);

    t.retardManuel += decalage;

    calculerDates();

    int nouvelle_duree_projet = 0;
    for (const auto& [_, task] : taches)
        nouvelle_duree_projet = std::max(nouvelle_duree_projet, task.fin_tot);

    std::cout << "\n=== Résultat du décalage ===\n";
    std::cout << "Tâche \"" << t.nom << "\" (ID " << id << ") décalée de " << decalage << " jour(s)\n";
    std::cout << "Début au plus tôt : " << t.debut_tot << "\n";
    std::cout << "Début au plus tard : " << t.debut_tard << "\n";
    std::cout << "Marge restante : " << t.marge << " jour(s)\n";
    std::cout << "Chemin critique : " << (t.critique ? "★ Oui" : "Non") << "\n";

    if (nouvelle_duree_projet > ancienne_duree_projet) {
        std::cout << "❗ La date minimale du projet est retardée : "
                  << nouvelle_duree_projet << " jours (avant " << ancienne_duree_projet << ")\n";
    } else if (nouvelle_duree_projet < ancienne_duree_projet) {
        std::cout << "✅ Projet raccourci : " << nouvelle_duree_projet << " jours\n";
    } else {
        std::cout << "✅ Ce décalage n’impacte pas la durée minimale du projet.\n";
    }

    std::cout << "============================\n";
}

// --- Fixer une date de début fixe ---
void GrapheOrienté::setDebutFixe(int id, int dateFixe) {
    if (taches.find(id) != taches.end()) {
        taches[id].debutFixe = dateFixe;
    }
}
