#include "graphe.h"
using namespace std;

// --- Ajout d’une tâche ---
void GrapheOrienté::ajouterTache(int id, const string &nom, int duree)
{
    taches[id] = {nom, duree};
}

// --- Ajout d’un arc orienté ---
void GrapheOrienté::ajouterArc(int source, int destination)
{
    taches[destination].dependances.push_back(source);
}

// --- Lecture du graphe depuis un fichier texte ---
void GrapheOrienté::chargerDepuisFichier(const string &nomFichier)
{
    taches.clear(); // Réinitialise le graphe avant de charger un nouveau fichier


    ifstream fichier(nomFichier);

    if (!fichier.is_open())
    {
        cerr << "Erreur : impossible d’ouvrir le fichier " << nomFichier << "\n";
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
            cerr << "Ligne ignorée (format invalide) : " << ligne << "\n";
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
    cout << " Graphe chargé depuis " << nomFichier << "\n";
}

// --- Affichage simple du graphe ---
void GrapheOrienté::afficher() const
{
    cout << "\n=== Graphe des dépendances ===\n";

    for (const pair<const int, Tache> &element : taches)
    {
        int id = element.first;
        const Tache &t = element.second;

        cout << "Tâche " << id << " (" << t.nom << ", " << t.duree << "j) dépend de : ";
        for (int d : t.dependances)
            cout << d << " ";
        cout << "\n";
    }
}

// --- Détection de cycle ---
bool GrapheOrienté::DetectCycle(int id, unordered_map<int, int> &etat) const
{
    etat[id] = 1; // tâche en cours de visite

    const vector<int> &dependances = taches.at(id).dependances;
    for (int dep : dependances)
    {
        if (etat[dep] == 1)  // cycle détecté
            return true; 
        if (etat[dep] == 0 && DetectCycle(dep, etat))  // appel récursif
            return true; 
    }

    etat[id] = 2;
    return false;
}

// --- Vérifie si le graphe est réalisable ---
bool GrapheOrienté::estRealisable() const
{
    unordered_map<int, int> etat;

    for (const pair<const int, Tache> &element : taches)
    {
        int id = element.first;
        etat[id] = 0;
    }

    for (const pair<const int, Tache> &element : taches)
    {
        int id = element.first;
        if (etat[id] == 0 && DetectCycle(id, etat))
            return false;
    }

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

    for (const pair<const int, Tache> &element : taches)
    {
        const Tache &t = element.second;

        if (find(t.dependances.begin(), t.dependances.end(), id) != t.dependances.end())
            min_debut = min(min_debut, t.debut_tard);
    }

    return (min_debut == INT_MAX) ? taches.at(id).fin_tot : min_debut;
}

// --- Modifier une tâche ---
void GrapheOrienté::modifierTache(int id, int nouvelleDuree, int decalageDuree)
{
    if (taches.find(id) == taches.end())
    {
        cout << "Tâche inexistante !\n";
        return;
    }

    Tache &t = taches[id];
    t.duree = nouvelleDuree;

    calculerDates();

    if (t.critique)
        cout << "  Attention ! La tâche \"" << t.nom
             << "\" est sur le chemin critique.\n";
    else
        cout << "Tâche modifiée : " << t.nom << "\n";
}

// --- Calcul des dates ---
void GrapheOrienté::calculerDates()
{
    if (!estRealisable())
    {
        cout << " Projet non réalisable (cycle détecté)\n";
        return;
    }

    vector<int> ordre;
    for (const pair<const int, Tache> &element : taches)
        ordre.push_back(element.first);
    sort(ordre.begin(), ordre.end());

    for (int id : ordre)
    {
        Tache &t = taches[id];
        t.debut_tot = maxFinPrecedentes(t.dependances) + t.retardManuel;
        t.fin_tot = t.debut_tot + t.duree;
    }

    int fin_projet = 0;
    for (int id : ordre)
        fin_projet = max(fin_projet, taches[id].fin_tot);

    for (int id : ordre)
    {
        Tache &t = taches[id];
        t.fin_tard = fin_projet;
        t.debut_tard = fin_projet - t.duree;
    }

    for (vector<int>::reverse_iterator it = ordre.rbegin(); it != ordre.rend(); ++it)
    {
        int id = *it;
        Tache &t = taches[id];
        t.fin_tard = minDebutSuivantes(id);
        t.debut_tard = t.fin_tard - t.duree;
    }

    for (pair<const int, Tache> &element : taches)
    {
        Tache &t = element.second;
        t.marge = t.debut_tard - t.debut_tot;
        t.critique = (t.marge == 0);
    }

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
             << " | " << (t.critique ? "*" : "") << "\n";
    }
    cout << "-------------------------------------------------------\n";
}

// --- Sauvegarde --- //faire en sorte qu'on demande  dans quel fichier sauvegarder au lieu de p1 automatiquement
void GrapheOrienté::sauvegarder(const string &nomFichier) const
{
    ofstream fichier(nomFichier);
    if (!fichier.is_open())
        throw runtime_error("Erreur : impossible d'ouvrir le fichier pour écriture.");

    // Récupérer et trier les identifiants
    vector<int> ids;
    ids.reserve(taches.size());
    for (const auto &p : taches)
        ids.push_back(p.first);
    sort(ids.begin(), ids.end());

    // Écriture dans l'ordre croissant
    for (int id : ids)
    {
        const Tache &t = taches.at(id);
        fichier << id << " " << t.nom << " " << t.duree << " ";

        if (t.dependances.empty()) {
            fichier << "-";
        } else {
            for (size_t i = 0; i < t.dependances.size(); ++i) {
                fichier << t.dependances[i];
                if (i != t.dependances.size() - 1)
                    fichier << " ";
            }
        }
        fichier << endl;
    }
    fichier.close();
    cout << "✅ Graphe sauvegardé dans " << nomFichier << " (ordre croissant)" << endl;
}

// --- Modifier le début d’une tâche ---
void GrapheOrienté::modifierDebutTache(int id, int decalage)
{
    if (taches.find(id) == taches.end())
    {
        cout << "Tâche inexistante !\n";
        return;
    }

    Tache &t = taches[id];

    calculerDates();

    int ancienne_duree_projet = 0;
    for (const pair<const int, Tache> &element : taches)
    {
        const Tache &task = element.second;
        ancienne_duree_projet = max(ancienne_duree_projet, task.fin_tot);
    }

    t.retardManuel += decalage;
    calculerDates();

    int nouvelle_duree_projet = 0;
    for (const pair<const int, Tache> &element : taches)
    {
        const Tache &task = element.second;
        nouvelle_duree_projet = max(nouvelle_duree_projet, task.fin_tot);
    }

    cout << "\n=== Résultat du décalage ===\n";
    cout << "Tâche \"" << t.nom << "\" (ID " << id << ") décalée de " << decalage << " jour(s)\n";
    cout << "Début au plus tôt : " << t.debut_tot << "\n";
    cout << "Début au plus tard : " << t.debut_tard << "\n";
    cout << "Marge restante : " << t.marge << " jour(s)\n";
    cout << "Chemin critique : " << (t.critique ? "* Oui" : "Non") << "\n";

    if (nouvelle_duree_projet > ancienne_duree_projet)
    {
        cout << "  Impossible : la date minimale du projet est désormais retardée !\n";
        cout << "Le projet durera " << nouvelle_duree_projet
             << " jours au lieu de " << ancienne_duree_projet << " jours.\n";
    }
    else if (nouvelle_duree_projet < ancienne_duree_projet)
    {
        cout << "Le projet est raccourci : "
             << nouvelle_duree_projet << " jours au lieu de " << ancienne_duree_projet << ".\n";
    }
    else
    {
        cout << "Ce décalage n’impacte pas la durée minimale du projet.\n";
    }

    cout << "============================\n";
}

// --- Fixer une date de début fixe ---
void GrapheOrienté::setDebutFixe(int id, int dateFixe)
{
    if (taches.find(id) != taches.end())
        taches[id].debutFixe = dateFixe;
}
