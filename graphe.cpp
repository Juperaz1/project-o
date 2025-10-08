#include "graphe.h"

// --- Lecture du graphe depuis un fichier texte ---
void GrapheOrienté::chargerDepuisFichier(const string &nomFichier)
{
    ifstream fichier(nomFichier);

    if (!fichier.is_open())
    {
        cerr << " Erreur : impossible d’ouvrir le fichier " << nomFichier << "\n";
        return;
    }

    string ligne;
    while (getline(fichier, ligne))
    {
        // Ignore les lignes vides ou commentaires
        if (ligne.empty() || ligne[0] == '#')
            continue;

        istringstream iss(ligne);
        int id;
        string nom;
        int duree;

        if (!(iss >> id >> nom >> duree))
        {
            cerr << "  Ligne ignorée (format invalide) : " << ligne << "\n";
            continue;
        }

        ajouterTache(id, nom, duree);

        string dep;
        while (iss >> dep)
        {
            if (dep == "-")
                continue;
            int idDep = stoi(dep);
            ajouterArc(idDep, id);
        }
    }

    fichier.close();
    cout << "✅ Graphe chargé depuis " << nomFichier << "\n";
}

// --- Ajout d’une tâche ---
<<<<<<< HEAD
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
=======
void GrapheOrienté::ajouterTache(int id, const string &nom, int duree)
{
    taches[id] = {nom, duree};
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
}

// --- Ajout d’un arc orienté ---
void GrapheOrienté::ajouterArc(int source, int destination)
{
    taches[destination].dependances.push_back(source);
}

// --- Affichage simple du graphe ---
void GrapheOrienté::afficher() const
{
    cout << "\n=== Graphe des dépendances ===\n";

<<<<<<< HEAD
    for (const auto& element : taches) { 
=======
    for (const pair<const int, Tache> &element : taches)
    {
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
        int id = element.first;
        const Tache &t = element.second;

        cout << "Tâche " << id << " (" << t.nom << ", " << t.duree << "j) dépend de : ";
        for (int d : t.dependances)
            cout << d << " ";
        cout << "\n";
    }
}

// --- Détection de cycle ---
<<<<<<< HEAD
bool GrapheOrienté::DetectCycle(int id, std::unordered_map<int, int>& etat) const {
    etat[id] = 1;

    for (int dep : taches.at(id).dependances) {
        if (etat[dep] == 1) return true;
        if (etat[dep] == 0 && DetectCycle(dep, etat)) return true;
=======
bool GrapheOrienté::DetectCycle(int id, unordered_map<int, int> &etat) const
{
    etat[id] = 1; // tâche en cours de visite

    const vector<int> &dependances = taches.at(id).dependances;
    for (int dep : dependances)
    { // Parcours des dépendances
        if (etat[dep] == 1)
            return true; // cycle détecté
        if (etat[dep] == 0 && DetectCycle(dep, etat))
            return true; // appel récursif
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
    }

    etat[id] = 2;
    return false;
}

bool GrapheOrienté::estRealisable() const
{
    unordered_map<int, int> etat;

<<<<<<< HEAD
    for (const auto& element : taches)
        etat[element.first] = 0;

    for (const auto& element : taches)
        if (etat[element.first] == 0 && DetectCycle(element.first, etat))
=======
    for (const pair<const int, Tache> &element : taches)
    {
        int id = element.first;
        etat[id] = 0;
    }

    for (const pair<const int, Tache> &element : taches)
    {
        int id = element.first;
        if (etat[id] == 0 && DetectCycle(id, etat))
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
            return false;

    return true;
}

// --- Calcul du max des fins précédentes ---
int GrapheOrienté::maxFinPrecedentes(const vector<int> &deps) const
{
    int max_fin = 0;
    for (int id : deps)
        max_fin = max(max_fin, taches.at(id).fin_tot);
    return max_fin;
}

// --- Calcul du min des débuts suivantes ---
int GrapheOrienté::minDebutSuivantes(int id) const
{
    int min_debut = INT_MAX;

<<<<<<< HEAD
    for (const auto& element : taches) {
        const Tache& t = element.second;
=======
    for (const pair<const int, Tache> &element : taches)
    {
        const int k = element.first;
        const Tache &t = element.second;
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)

        if (find(t.dependances.begin(), t.dependances.end(), id) != t.dependances.end())
        {
            min_debut = min(min_debut, t.debut_tard);
        }
    }

    return (min_debut == INT_MAX) ? taches.at(id).fin_tot : min_debut;
}

// --- Modifier une tâche et vérifier impact ---
<<<<<<< HEAD
void GrapheOrienté::modifierTache(int id, int nouvelleDuree, int decalageDuree) {
    if (taches.find(id) == taches.end()) {
        std::cout << "⚠️  Tâche inexistante !\n";
        return;
    }

    Tache& t = taches[id];
=======
void GrapheOrienté::modifierTache(int id, int nouvelleDuree, int decalageDuree = 0)
{
    if (taches.find(id) == taches.end())
    {
        cout << "  Tâche inexistante !\n";
        return;
    }

    Tache &t = taches[id];

    // On change seulement la durée si besoin
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
    t.duree = nouvelleDuree;

    calculerDates();

<<<<<<< HEAD
    if (t.critique) {
        std::cout << "❌ Attention ! La tâche \"" << t.nom 
                  << "\" est sur le chemin critique, tout décalage retardera le projet !\n";
    } else {
        std::cout << "✅ Tâche modifiée : " << t.nom << "\n";
=======
    // Vérifie si la tâche est critique après recalcul
    if (t.critique)
    {
        cout << " Attention ! La tâche \"" << t.nom
             << "\" est sur le chemin critique, tout décalage retardera le projet !\n";
    }
    else
    {
        cout << " Tâche modifiée : " << t.nom << "\n";
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
    }
}

// --- Calcul des dates au plus tôt / au plus tard / marges ---
<<<<<<< HEAD
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

=======
void GrapheOrienté::calculerDates()
{
    // Vérifie d’abord si le graphe est valide
    if (!estRealisable())
    {
        cout << "⚠️  Projet non réalisable (cycle détecté)\n";
        return;
    }

    // Tri topologique simple (manuel ici)
    vector<int> ordre;
    for (const pair<const int, Tache> &element : taches)
    {
        int id = element.first;
        ordre.push_back(id);
    }
    sort(ordre.begin(), ordre.end());

    // --- Calcul des dates au plus tôt ---
    for (int id : ordre)
    {
        Tache &t = taches[id];
        t.debut_tot = max(maxFinPrecedentes(t.dependances), t.debut_tot) + t.retardManuel;
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
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
        fin_projet = max(fin_projet, taches[id].fin_tot);

<<<<<<< HEAD
    // --- Étape 5 : Initialiser au plus tard à la fin du projet ---
    for (int id : ordre) {
        Tache& t = taches[id];
=======
    // --- Initialisation des dates au plus tard ---
    for (int id : ordre)
    {
        Tache &t = taches[id];
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
        t.fin_tard = fin_projet;
        t.debut_tard = fin_projet - t.duree;
    }

<<<<<<< HEAD
    // --- Étape 6 : Calcul des dates au plus tard (retour) ---
    for (auto it = ordre.rbegin(); it != ordre.rend(); ++it) {
=======
    // --- Calcul des dates au plus tard (retour en arrière) ---
    for (vector<int>::reverse_iterator it = ordre.rbegin(); it != ordre.rend(); ++it)
    {
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
        int id = *it;
        Tache &t = taches[id];
        t.fin_tard = minDebutSuivantes(id);
        t.debut_tard = t.fin_tard - t.duree;
    }

<<<<<<< HEAD
    // --- Étape 7 : Marges et critiques ---
    for (auto& [_, t] : taches) {
=======
    // --- Calcul des marges et tâches critiques ---
    for (pair<const int, Tache> &element : taches)
    {
        int id = element.first;
        Tache &t = element.second;

>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
        t.marge = t.debut_tard - t.debut_tot;
        t.critique = (t.marge == 0);
    }

    // --- Affichage ---
<<<<<<< HEAD
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
=======
    cout << "\nID | Tâche | Début+Tôt | Début+Tard | Marge | Critique\n";
    cout << "-------------------------------------------------------\n";
    for (int id : ordre)
    {
        const Tache &t = taches.at(id);
        cout << id << " | " << t.nom;
        int spaces = max(0, 12 - (int)t.nom.size());
        cout << string(spaces, ' ')
             << "| " << t.debut_tot
             << " | " << t.debut_tard
             << " | " << t.marge
             << " | " << (t.critique ? "★" : "") << "\n";
    }
    cout << "-------------------------------------------------------\n";

    /* Chemin critique */
    vector<int> cheminCritique;
    for (pair<const int, Tache> &element : taches)
    {
        int id = element.first;
        Tache &t = element.second;

        t.marge = t.debut_tard - t.debut_tot;
        t.critique = (t.marge == 0);

        if (t.critique)
        {
            cheminCritique.push_back(id);
        }
    }

    reverse(cheminCritique.begin(), cheminCritique.end());
    cout << "Chemin critique : ";
    for (size_t i = 0; i < cheminCritique.size(); ++i)
    {
        if (i > 0)
        {
            cout << " -> ";
        }
        cout << cheminCritique[i];
    }
    cout << endl;

    /* Tâches pouvant être retardées */
    vector<int> margeTaches;
    for (pair<const int, Tache> &element : taches)
    {
        int id = element.first;
        Tache &t = element.second;

        if (t.debut_tard - t.debut_tot != 0)
        {
            margeTaches.push_back(id);
        }
    }
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)

    reverse(margeTaches.begin(), margeTaches.end());
    cout << "Tâches avec marge : ";
    for (size_t i = 0; i < margeTaches.size(); ++i)
    {
        int id = margeTaches[i];
<<<<<<< HEAD
        const Tache& t = taches.at(id);
        std::cout << id << "(" << t.marge << "j)";
        if (i < margeTaches.size() - 1) std::cout << "; ";
=======
        Tache &t = taches[id];
        cout << id << "(" << t.marge << "j)";
        if (i < margeTaches.size() - 1)
        {
            cout << "; ";
        }
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
    }
    std::cout << "\n";

<<<<<<< HEAD
    std::cout << "\nDurée minimale du projet : " << fin_projet << " jours\n";
}


// --- Décaler une tâche ---
void GrapheOrienté::modifierDebutTache(int id, int decalage) {
    if (taches.find(id) == taches.end()) {
        std::cout << "⚠️ Tâche inexistante !\n";
=======
    cout << endl;
    cout << "\nDurée minimale du projet : " << fin_projet << " jours\n";
}

void GrapheOrienté::sauvegarder(const std::string& nomFichier) const {
    ofstream fichier(nomFichier);
    if (!fichier.is_open())
        throw runtime_error("Erreur : impossible d'ouvrir le fichier pour écriture.");

    fichier << taches.size() << endl;
    for (const auto& [id, t] : taches) {
        // Écriture du nom sans guillemets, en séparant par tabulation
        fichier << id << "\t" << t.nom << "\t" << t.duree
                << "\t" << t.dependances.size();
        for (int dep : t.dependances)
            fichier << "\t" << dep;
        fichier << endl;
    }
    fichier.close();
    cout << "Graphe sauvegardé dans " << nomFichier << endl;
}

void GrapheOrienté::modifierDebutTache(int id, int decalage)
{
    if (taches.find(id) == taches.end())
    {
        cout << " Tâche inexistante !\n";
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
        return;
    }

    Tache &t = taches[id];

    calculerDates();
    int ancienne_duree_projet = 0;
    for (const auto &[_, task] : taches)
        ancienne_duree_projet = max(ancienne_duree_projet, task.fin_tot);

    t.retardManuel += decalage;

    calculerDates();

    int nouvelle_duree_projet = 0;
    for (const auto &[_, task] : taches)
        nouvelle_duree_projet = max(nouvelle_duree_projet, task.fin_tot);

<<<<<<< HEAD
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
=======
    // 5. Affichage
    cout << "\n=== Résultat du décalage ===\n";
    cout << "Tâche \"" << t.nom << "\" (ID " << id << ") décalée de " << decalage << " jour(s)\n";
    cout << "Début au plus tôt : " << t.debut_tot << "\n";
    cout << "Début au plus tard : " << t.debut_tard << "\n";
    cout << "Marge restante : " << t.marge << " jour(s)\n";
    cout << "Chemin critique : " << (t.critique ? "★ Oui" : "Non") << "\n";

    if (nouvelle_duree_projet > ancienne_duree_projet)
    {
        cout << " Impossible : la date minimale du projet est désormais retardée !\n";
        cout << " Le projet durera " << nouvelle_duree_projet
             << " jours au lieu de " << ancienne_duree_projet << " jours.\n";
    }
    else if (nouvelle_duree_projet < ancienne_duree_projet)
    {
        cout << " Le projet est raccourci : "
             << nouvelle_duree_projet << " jours au lieu de " << ancienne_duree_projet << ".\n";
    }
    else
    {
        cout << " Ce décalage n’impacte pas la durée minimale du projet.\n";
>>>>>>> 67c54d8 (Sauvegarde/ ajout du charger (fonctionnel) sauvergarder (non fonctionnel et menu)
    }

    cout << "============================\n";
}

// --- Fixer une date de début fixe ---
void GrapheOrienté::setDebutFixe(int id, int dateFixe) {
    if (taches.find(id) != taches.end()) {
        taches[id].debutFixe = dateFixe;
    }
}
