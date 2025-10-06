#include "graphe.h"

int main() {
    GrapheOrienté projet;

    // --- Définition des tâches du projet P1 ---
    projet.ajouterTache(1, "Murs", 7);
    projet.ajouterTache(2, "Charpente", 3);
    projet.ajouterTache(3, "Toiture", 1);
    projet.ajouterTache(4, "Plomberie", 8);
    projet.ajouterTache(5, "Électricité", 2);
    projet.ajouterTache(6, "Fenêtres", 1);
    projet.ajouterTache(7, "Jardin", 1);
    projet.ajouterTache(8, "Peintures", 3);
    projet.ajouterTache(9, "Cuisine", 2);
    projet.ajouterTache(10, "Installation", 1);

    // --- Dépendances (arcs orientés) ---
    projet.ajouterArc(1, 2);
    projet.ajouterArc(2, 3);
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

    if (projet.estRealisable()) {
        projet.calculerDates();
    } else {
        std::cout << "Le projet contient des dépendances circulaires !\n";
    }

    return 0;
}
