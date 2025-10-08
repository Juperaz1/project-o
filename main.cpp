#include "graphe.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

bool fichierExiste(const string &chemin)
{
    ifstream f(chemin);
    return f.good();
}

// Crée le dossier si besoin
void creerDossier(const string &nomDossier) {
    int ret = mkdir(nomDossier.c_str(), 0777);
    if(ret != 0 && errno != EEXIST)
    {
        cerr << "Impossible de créer le dossier '" << nomDossier << "'" << endl;
    }
}


string choisirFichierSauvegarde() {
    string dossier = "../graphe";   // au même niveau que build
    creerDossier(dossier);

    string nomFichier;
    cout << "Entrez le nom du fichier (sans extension) : ";
    cin >> nomFichier;

    // Construire le chemin complet avec extension
    string cheminComplet = dossier + "/" + nomFichier;
    if (cheminComplet.size() < 4 || cheminComplet.substr(cheminComplet.size() - 4) != ".txt")
        cheminComplet += ".txt";

    // Vérifier si le fichier existe
    if (fichierExiste(cheminComplet)) {
        char choix;
        cout << "Le fichier existe déjà. Voulez-vous l'écraser ? (o/n) : ";
        cin >> choix;
        if (choix != 'o' && choix != 'O') {
            cout << "Abandon de la sauvegarde." << endl;
            return "";
        }
    }
    return cheminComplet;
}

int main(int argc, char *argv[])
{
    cout << " Entrez un chemin d'un fichier TXT contenant un graphe dans le format correct : ";
    string nomFichier;
    cin >> nomFichier;
    GrapheOrienté projet;

    projet.chargerDepuisFichier(nomFichier);
    projet.afficher();

    if (projet.estRealisable())
    {
        projet.calculerDates();
    }
    else
    {
        cout << "  Projet non réalisable : cycle détecté.\n";
    }

    int menu = 1;

    while (menu == 1)
    {

        int valmenu;
        cout << " 1. Décaler une tache \n 2. Sauvegarder le graphe dans un fichier \n 3. Charger un nouveau graphe (depuis un .txt) \n 0. Quitter \n ";
        cin >> valmenu;
        switch (valmenu)
        {
        case 1:
        {
            int id, decalage;
            cout << "\nEntrez l'ID de la tâche à décaler (0 pour quitter) : ";
            cin >> id;

            if (id == 0)
            {
                cout << " Opération annulée par l'utilisateur.\n";
                return 0;
            }

            cout << "Entrez le nombre de jours de décalage (positif = retard, négatif = avancer) : ";
            cin >> decalage;
            projet.modifierDebutTache(id, decalage);
            break;
        }
        case 2:
        {
            string fichier = choisirFichierSauvegarde();
            if(!fichier.empty())
            {
                try{
                    projet.sauvegarder(fichier);
                }
                catch (const runtime_error &e)
                {
                    cerr << e.what() << endl;
                }
            }
            break;
        }
        case 3:
        {
            cout << " Entrez un chemin d'un fichier TXT contenant un graphe dans le format correct : ";
            cin >> nomFichier;
            projet.chargerDepuisFichier(nomFichier);
            projet.afficher();

            if (projet.estRealisable())
            {
                projet.calculerDates();
            }
            else
            {
                cout << "  Projet non réalisable : cycle détecté.\n";
            }
            break;
        }
        case 0:
        {
            return 0;
        }
        default:
        {
            cout << "Entrez une valeur valide\n";
        }
        }
    }

    return 0;
}