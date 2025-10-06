#include "graphe.h"

int main() {
    GrapheOrienté projet;

    // --- Ajout des tâches ---
    projet.ajouterTache(1, 7);
    projet.ajouterTache(2, 3);
    projet.ajouterTache(3, 1);
    projet.ajouterTache(4, 8);
    projet.ajouterTache(5, 2);
    projet.ajouterTache(6, 1);
    projet.ajouterTache(7, 1);
    projet.ajouterTache(8, 3);
    projet.ajouterTache(9, 2);
    projet.ajouterTache(10, 1);

    // --- Ajout des arcs (dépendances) ---
    projet.ajouterArc(1, 2);
    projet.ajouterArc(2, 1);
    projet.ajouterArc(1, 4);
    projet.ajouterArc(3, 5);
    projet.ajouterArc(4, 5);
    projet.ajouterArc(3, 6);
    projet.ajouterArc(4, 6);
    projet.ajouterArc(3, 7);
    projet.ajouterArc(4, 7);
    projet.ajouterArc(6, 8);
    projet.ajouterArc(8, 9);
    projet.ajouterArc(5, 10);
    projet.ajouterArc(7, 10);
    projet.ajouterArc(9, 10);

    projet.afficher();

    projet.estRealisable();
}
