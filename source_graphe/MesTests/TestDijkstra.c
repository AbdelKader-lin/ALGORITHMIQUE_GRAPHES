#include "../graphe.h"  // Assurez-vous que ce fichier contient la définition de pgraphe_t et les prototypes nécessaires
#include <stdio.h>
#include <stdlib.h>

// Fonction pour charger le graphe et exécuter Dijkstra
void run_dijkstra_test(pgraphe_t graph, int source_vertex, int* expected_distances) {
    algo_dijkstra(graph, source_vertex);

    // Vérifiez si les distances calculées correspondent aux distances attendues
    psommet_t current = graph;
    while (current != NULL) {
        int expected = expected_distances[current->label];
        if (current->distance != expected) {
            printf("Test Dijkstra échoué au sommet %d: attendu %d, obtenu %d\n", current->label, expected, current->distance);
            return;
        }
        current = current->sommet_suivant;
    }
    printf("Test Dijkstra réussi.\n");
}

// Test pour le graphe gr5
void test_dijkstra_gr5() {
    pgraphe_t graph = load_graph_from_file("../data/gr5");  // Charge le graphe gr5
    int expected_distances[] = {0, 2, 5, 7};  // Distances attendues pour chaque sommet depuis la source
    run_dijkstra_test(graph, 0, expected_distances);  // Sommet source 0
}

// Test pour le graphe gr6
void test_dijkstra_gr6() {
    pgraphe_t graph = load_graph_from_file("../data/gr6");  // Charge le graphe gr6
    int expected_distances[] = {3, 0, 2, 6};  // Distances attendues pour chaque sommet depuis la source
    run_dijkstra_test(graph, 1, expected_distances);  // Sommet source 1
}

int main() {
    test_dijkstra_gr5();
    test_dijkstra_gr6();
    printf("TESTS DIJKSTRA RÉUSSIS !\n");
    return 0;
}
