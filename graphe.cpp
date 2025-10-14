#include "graphe.h"
#include <iomanip> // ✅ pour setw, setfill, etc.
using namespace std;

// --- Ajout d’une tâche ---
void GrapheOrienté::ajouterTache(int id, const string &nom, int duree, int debut_min)
{
    Tache t;
    t.nom = nom;
    t.duree = duree;
    t.debut_min = debut_min;
    taches[id] = t;
}


// --- Ajout d’un arc orienté ---
void GrapheOrienté::ajouterArc(int source, int destination)
{
    // Ne rien ajouter ici, cette fonction sert juste pour la logique si besoin plus tard.
    // Elle ne doit pas modifier la structure.
}


// --- Lecture du graphe depuis un fichier texte ---
void GrapheOrienté::chargerDepuisFichier(const string &nomFichier)
{
    taches.clear();

    ifstream fichier(nomFichier);
    if (!fichier.is_open()) {
        cerr << "Erreur : impossible d’ouvrir le fichier " << nomFichier << "\n";
        return;
    }

    string ligne;
    bool formatP2 = false;

    // --- Détection du format (cherche "debut_min" dans l'en-tête si présent) ---
    streampos posDebut = fichier.tellg();
    while (getline(fichier, ligne)) {
        if (ligne.empty()) continue;
        if (ligne[0] == '#') {
            if (ligne.find("debut_min") != string::npos) {
                formatP2 = true;
            }
            // si c'est un commentaire on continue la détection
            continue;
        }
        // première ligne non-commentaire rencontrée, stop
        break;
    }
    // revenir au début du fichier pour relire toutes les lignes
    fichier.clear();
    fichier.seekg(0);
    
    // --- Lecture réelle ---
    while (getline(fichier, ligne)) {
        if (ligne.empty()) continue;
        if (ligne[0] == '#') continue; // ignorer commentaires

        istringstream iss(ligne);
        int id = 0, duree = 0;
        string nom;
        int debut_min = 0;

        string depsToken = "-";
        string chevToken = "-";

        if (formatP2) {
            // Format attendu : id nom duree debut_min dependances chevauchements
            // dépendances et chevauchements doivent être des tokens (ex: "1,2" ou "-" pour vide)
            if (!(iss >> id >> nom >> duree >> debut_min)) {
                cerr << "Ligne ignorée (format P2 invalide) : " << ligne << "\n";
                continue;
            }
            if (!(iss >> depsToken)) depsToken = "-";
            if (!(iss >> chevToken)) chevToken = "-";
        } else {
            // Format P1 : id nom duree dependances(0..n)
            // Ici les dépendances peuvent être plusieurs entiers séparés par des espaces
            if (!(iss >> id >> nom >> duree)) {
                cerr << "Ligne ignorée (format P1 invalide) : " << ligne << "\n";
                continue;
            }
            // récupérer le reste de la ligne (tout ce qui suit) comme dépendances
            string rest;
            getline(iss, rest); // lit jusqu'à la fin de la ligne
            // trim gauche des espaces
            size_t first = rest.find_first_not_of(" \t");
            if (first == string::npos) {
                depsToken = "-";
            } else {
                depsToken = rest.substr(first);
            }
            // chevToken reste "-" pour P1
            chevToken = "-";
        }

        // Créer la tâche
        ajouterTache(id, nom, duree, debut_min);
        Tache &t = taches[id];

        // --- Traiter les dépendances : remplacer ',' par ' ' puis lire tous les ints ---
        if (!depsToken.empty() && depsToken != "-") {
            // remplacer les virgules par des espaces
            replace(depsToken.begin(), depsToken.end(), ',', ' ');
            istringstream ss(depsToken);
            int val;
            // s'assurer d'éviter doublons (au cas où)
            while (ss >> val) {
                if (find(t.dependances.begin(), t.dependances.end(), val) == t.dependances.end()) {
                    t.dependances.push_back(val);
                }
            }
        }

        // --- Traiter les chevauchements (P2 seulement) ---
        if (!chevToken.empty() && chevToken != "-") {
            replace(chevToken.begin(), chevToken.end(), ',', ' ');
            istringstream ss2(chevToken);
            int val;
            while (ss2 >> val) {
                if (find(t.chevauchements.begin(), t.chevauchements.end(), val) == t.chevauchements.end()) {
                    t.chevauchements.push_back(val);
                }
            }
        }
    }

    fichier.close();
    cout << "✅ Graphe chargé depuis " << nomFichier
         << (formatP2 ? " (format P2)" : " (format P1)") << "\n";
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

    cout << "\nID | Tâche        | Durée | Début_min | Début+Tôt | Début+Tard | Marge | Critique | Chevauchements\n";
    cout << "----------------------------------------------------------------------------------------------------\n";
    for (int id : ordre)
    {
        const Tache &t = taches.at(id);

        cout << setw(2) << id << " | "
             << left << setw(12) << t.nom << " | "
             << setw(5) << t.duree << " | "
             << setw(10) << t.debut_min << " | "
             << setw(10) << t.debut_tot << " | "
             << setw(11) << t.debut_tard << " | "
             << setw(5) << t.marge << " | "
             << (t.critique ? "*" : " ") << " | ";

        // Affichage des chevauchements
        if (t.chevauchements.empty())
            cout << "-";
        else
        {
            for (size_t i = 0; i < t.chevauchements.size(); ++i)
            {
                cout << t.chevauchements[i];
                if (i != t.chevauchements.size() - 1)
                    cout << ",";
            }
        }

        cout << "\n";
    }
    cout << "----------------------------------------------------------------------------------------------------\n";

    /* Chemin critique */
    vector<int> cheminCritique;
    for(map<int, Tache>::iterator it = taches.begin(); it != taches.end(); ++it)
    {
        int id = it->first;
        Tache &t = it->second;
        t.marge = t.debut_tard - t.debut_tot;
        t.critique = (t.marge == 0);
        if(t.critique)
        {
            cheminCritique.push_back(id);
        }
    }
    cout << "Chemin critique : ";
    for(size_t i = 0; i < cheminCritique.size(); ++i)
    {
        if(i > 0)
        {
            cout << " -> ";
        }
        cout << cheminCritique[i];
    }
    cout << std::endl;

    /* Tâches avec marge */
    vector<int> Marge;

    for(map<int, Tache>::iterator it = taches.begin(); it != taches.end(); ++it)
    {
        int id = it->first;
        Tache &t = it->second;
        t.marge = t.debut_tard - t.debut_tot;
        if(t.marge != 0)
        {
            Marge.push_back(id);
        }
    }

    cout << "Tâche avec marge : ";
    for(size_t i = 0; i < Marge.size(); ++i)
    {
        int id = Marge[i];
        Tache &t = taches[id];
        cout << id << "(" << t.marge << "j)";
        if(i < Marge.size() - 1)
        {
            cout << "; ";
        }
    }
    cout << std::endl;
    cout << "Durée minimale du projet : " << fin_projet << " jours" << endl << endl;
}

/* Sauvegarde */
void GrapheOrienté::sauvegarder(const std::string& nomFichier) const
{
    ofstream fichier(nomFichier);
    if (!fichier.is_open()) {
        throw runtime_error("Erreur : impossible d’ouvrir le fichier pour écriture.");
    }

    // Vérifier si au moins une tâche contient des champs avancés (P2)
    bool formatP2 = false;
    for (const auto& [id, t] : taches) {
        if (t.debut_min != 0 || !t.chevauchements.empty()) {
            formatP2 = true;
            break;
        }
    }

    // Écriture de l’en-tête (facultative pour compatibilité)
    if (formatP2)
        fichier << "# id nom duree debut_min dependances chevauchements\n";
    else
        fichier << "# id nom duree dependances\n";

    // Trier les identifiants pour sauvegarde ordonnée
    vector<int> ids;
    ids.reserve(taches.size());
    for (const auto& p : taches) ids.push_back(p.first);
    sort(ids.begin(), ids.end());

    // Parcourir les tâches et écrire leurs infos
    for (int id : ids) {
        const Tache& t = taches.at(id);

        // Format commun
        fichier << id << " " << t.nom << " " << t.duree << " ";

        if (formatP2) {
            // --- FORMAT P2 (plus complet) ---
            fichier << t.debut_min << " ";

            // Dépendances
            if (t.dependances.empty()) fichier << "- ";
            else {
                for (size_t i = 0; i < t.dependances.size(); ++i) {
                    fichier << t.dependances[i];
                    if (i != t.dependances.size() - 1) fichier << ",";
                }
                fichier << " ";
            }

            // Chevauchements
            if (t.chevauchements.empty()) fichier << "-";
            else {
                for (size_t i = 0; i < t.chevauchements.size(); ++i) {
                    fichier << t.chevauchements[i];
                    if (i != t.chevauchements.size() - 1) fichier << ",";
                }
            }
        } else {
            // --- FORMAT P1 (ancien) ---
            if (t.dependances.empty()) {
                fichier << "-";
            } else {
                for (size_t i = 0; i < t.dependances.size(); ++i) {
                    fichier << t.dependances[i];
                    if (i != t.dependances.size() - 1) fichier << " ";
                }
            }
        }

        fichier << "\n";
    }

    fichier.close();

    cout << "✅ Graphe sauvegardé dans " << nomFichier 
         << " (" << (formatP2 ? "format P2" : "format P1") << ")" << endl;
}

void GrapheOrienté::creerChevauchement(int id1, int id2)
{
    Tache &t1 = taches[id1];
    Tache &t2 = taches[id2];

    bool chevauchementExistant1 = false;
    for(size_t j = 0; j < t1.chevauchements.size(); j++)
    {
        if(t1.chevauchements[j] == id2)
        {
            chevauchementExistant1 = true;
            break;
        }
    }
    if(!chevauchementExistant1)
    {
        t1.chevauchements.push_back(id2);
    }
    bool chevauchementExistant2 = false;
    for(size_t j = 0; j < t2.chevauchements.size(); j++)
    {
        if(t2.chevauchements[j] == id1)
        {
            chevauchementExistant2 = true;
            break;
        }
    }
    if(!chevauchementExistant2)
    {
        t2.chevauchements.push_back(id1);
    }
    cout << "Chevauchement créé entre Tâche " << id1 << " et Tâche " << id2 << "." << endl;
}

void GrapheOrienté::fixerDebutMin(int id, int jour)
{
    if(taches.find(id) != taches.end())
    {
        taches[id].debut_min = jour;
        cout << "Début minimum de la tâche " << id << " fixé au jour " << jour << "." << endl;
    }
    else
    {
        cout << "Erreur : la tâche avec l'ID " << id << " n'existe pas." << endl;
    }
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
