#include "graphe.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << " Utilisation : " << argv[0] << " <nom_du_fichier.txt>\n";
        return 1;
    }

    string nomFichier = argv[1];
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
        cout << "1. Décaler une tache \n 2. Sauvegarder le graphe dans un fichier \n 3.  Changer le graphe \n 0. quitter \n ";
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
                return 0; // ou continue / break selon ton usage
            }

            cout << "Entrez le nombre de jours de décalage (positif = retard, négatif = avancer) : ";
            cin >> decalage;
            projet.modifierDebutTache(id, decalage);
            break;
        }
        case 2:
        {
            projet.sauvegarder(nomFichier);
            break;
        }
        case 3:
        {
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