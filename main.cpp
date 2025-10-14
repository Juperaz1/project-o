#include "graphe.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

/* Vérifie si le fichier existe */
bool fichierExiste(const string &chemin)
{
    ifstream f(chemin);
    return f.good();
}

/* Crée le dossier si besoin */
void creerDossier(const string &nomDossier)
{
    int ret = mkdir(nomDossier.c_str(), 0777);
    if(ret != 0 && errno != EEXIST)
    {
        cerr << "Impossible de créer le dossier '" << nomDossier << "'" << endl;
    }
}

string choisirFichierSauvegarde()
{
    string dossier = "../graphe";
    creerDossier(dossier);
    string nomFichier;
    cout << "Entrez le nom du fichier (sans extension) : ";
    cin >> nomFichier;

    // Construire le chemin complet avec extension
    string cheminComplet = dossier + "/" + nomFichier;
    if(cheminComplet.size() < 4 || cheminComplet.substr(cheminComplet.size() - 4) != ".txt")
    {
        cheminComplet += ".txt";
    }
    // Vérifier si le fichier existe
    if(fichierExiste(cheminComplet))
    {
        char choix;
        cout << "Le fichier existe déjà. Voulez-vous l'écraser ? (o/n) : ";
        cin >> choix;
        if(choix != 'o' && choix != 'O')
        {
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
    if(projet.estRealisable())
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
        cout << " 1. Décaler une tache \n 2. Sauvegarder le graphe dans un fichier \n 3. Charger un nouveau graphe (depuis un .txt) \n 4. Ajouter une contrainte \n 0. Quitter \n ";
        cin >> valmenu;
        switch(valmenu)
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
                catch(const runtime_error &e)
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
        case 4:
        {
            while(valmenu == 4)
            {
                int choix;
                cout << " 1. Ajouter une dépendance \n 2. Imposer une date minimum \n 3. Ajouter un chevauchement \n 0. Quitter \n ";
                cin >> choix;
                switch (choix)
                {
                    case 1:
                    {
                        int source, destination;
                        cout << "\n=== Ajout d'une contrainte ===" << endl;
                        cout << "Entrez l'ID de la tâche source (précédente) : ";
                        cin >> source;
                        cout << "Entrez l'ID de la tâche destination (suivante) : ";
                        cin >> destination;
                        if(source == destination)
                        {
                            cout << "Erreur : une tâche ne peut pas dépendre d’elle-même.\n";
                        }
                        else
                        {
                            projet.ajouterArc(source, destination);
                            cout << "Dépendance ajoutée : Tâche " << destination << " dépend désormais de la tâche " << source << ".\n";
                            projet.afficher();
                            if(projet.estRealisable())
                            {
                                projet.calculerDates();
                            }
                            else
                            {
                                cout << "Projet non réalisable après ajout de la dépendance (cycle détecté).\n";
                            }
                        }
                    case 2:
                    {
                        int id, jour;
                        cout << "Entrez l'ID de la tâche à modifier : ";
                        cin >> id;
                        cout << "Entrez le jour minimum auquel la tâche peut commencer : ";
                        cin >> jour;
                        projet.fixerDebutMin(id, jour);
                        break;
                    }
                    case 3:
                    {
                        int id1, id2;
                        cout << "Entrez l'ID de la première tâche : ";
                        cin >> id1;
                        cout << "Entrez l'ID de la deuxième tâche : ";
                        cin >> id2;
                        projet.creerChevauchement(id1, id2);
                        break;
                    }
                    case 0:
                    {
                        valmenu = 0;
                        break;
                    }
                    default:
                    {
                        cout << "Choix invalide\n";
                    }
                    }
                }
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