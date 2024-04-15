#include "../graphe.h"  // Assurez-vous que ce fichier contient la définition de pgraphe_t et les prototypes nécessaires
#include <stdio.h>
#include <stdlib.h>
#include "../abr.h" 


pgraphe_t load_graph_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }

    pgraphe_t graphe = NULL;
    int label, distance, label_dest;
    
    while (!feof(file)) {
        if (fscanf(file, "%d", &label) != 1) break;  // Lire le label du sommet

        psommet_t sommet = chercher_sommet(graphe, label);
        if (sommet == NULL) {  // Si le sommet n'existe pas, créez-le.
            sommet = (psommet_t)malloc(sizeof(sommet_t));
            if (!sommet) {
                fprintf(stderr, "Erreur de mémoire\n");
                exit(EXIT_FAILURE);
            }
            sommet->label = label;
            sommet->liste_arcs = NULL;
            sommet->sommet_suivant = graphe;  // Ajouter en tête de la liste
            graphe = sommet;
        }

        // Lire les arcs
        while (fscanf(file, "%d %d", &label_dest, &distance) == 2) {
            psommet_t dest = chercher_sommet(graphe, label_dest);
            if (dest == NULL) {  // Si le sommet de destination n'existe pas, créez-le.
                dest = (psommet_t)malloc(sizeof(sommet_t));
                if (!dest) {
                    fprintf(stderr, "Erreur de mémoire\n");
                    exit(EXIT_FAILURE);
                }
                dest->label = label_dest;
                dest->liste_arcs = NULL;
                dest->sommet_suivant = graphe;
                graphe = dest;
            }
            // Ajouter un arc
            ajouter_arc(sommet, dest, distance);
        }
    }

    fclose(file);
    return graphe;
}


// Fonction pour charger le graphe et exécuter Dijkstra
void run_dijkstra_test(pgraphe_t graph, int source_vertex, int* expected_distances) {
    algo_dijkstra(graph, source_vertex);

    // Vérifiez si les distances calculées correspondent aux distances attendues
    psommet_t current = graph;
    while (current != NULL) {
        int expected = expected_distances[current->label];
        if (current->distance != expected) {
            printf("Test Dijkstra échoué au sommet %d: attendu %d, obtenu %d\n", current->label, expected, current->distance);
            exit(EXIT_FAILURE);  // Quitter en cas d'échec du test
        }
        current = current->sommet_suivant;
    }
    printf("Test Dijkstra réussi pour le sommet source %d.\n", source_vertex);
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
    printf("Début des tests Dijkstra...\n");
    test_dijkstra_gr5();
    test_dijkstra_gr6();
    printf("Tous les tests Dijkstra ont réussi !\n");
    return EXIT_SUCCESS;
}
