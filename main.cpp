#include "graphe.h"

int main() {
    GrapheOrienté projet;

    // --- Définition des tâches du projet P2 ---
    projet.ajouterTache(1, "Tâche 1", 1);
    projet.ajouterTache(2, "Tâche 2", 3); // doit commencer exactement à 3
    projet.ajouterTache(3, "Tâche 3", 1);
    projet.ajouterTache(4, "Tâche 4", 2); // commence après T1 & T2
    projet.ajouterTache(5, "Tâche 5", 1); // commence après début de T3

    /* --- Anciennes valeurs P1 ---
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
    */

    // --- Dépendances / contraintes spécifiques P2 ---
    projet.ajouterArc(1, 4); // ✅ T4 après T1
    projet.ajouterArc(2, 4); // ✅ T4 après T2
    // ❌ NE PAS faire de projet.ajouterArc(3, 5) : ce n'est pas une dépendance stricte
    
    // Pour le chevauchement T3 & T4 ≥1 unité :
    // On peut gérer ça dans la logique de calcul de dates si le GrapheOrienté supporte les contraintes supplémentaires
    // Sinon, on placera T3 à 6 pour chevaucher T4

    /* --- Anciennes dépendances P1 ---
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
    */

    // --- Affichage initial ---
    projet.afficher();

    // --- Fixer T2 au jour 3 ---
    projet.setDebutFixe(2, 3); 

    // --- Calcul des dates et vérification de la réalisabilité ---
    if (projet.estRealisable()) {
        projet.calculerDates();
    } else {
        std::cout << "Le projet contient des dépendances circulaires !\n";
    }
    
    // --- Interaction pour décaler une tâche ---
    int id, decalage;
    std::cout << "\nEntrez l'ID de la tâche à décaler (0 pour quitter) : ";
    std::cin >> id;

    if (id == 0) {
        std::cout << "❌ Opération annulée par l'utilisateur.\n";
        return 0; 
    }

    std::cout << "Entrez le nombre de jours de décalage (positif = retard, négatif = avancer) : ";
    std::cin >> decalage;

    projet.modifierDebutTache(id, decalage);

    // Re-fixer T2 au jour 3 si jamais T2 a été déplacée
    projet.setDebutFixe(2, 3);

    return 0;
}
