/*
  Structures de type graphe
  Structures de donnees de type liste
  (Pas de contrainte sur le nombre de noeuds des  graphes)
*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Pour INT_MAX

#include "graphe.h"
#include "file.h"
#include "pile.h"

#define Entier_max 1000



/* PSEUDO-CODE


Fonction dijkstra(g: pgraphe_t, r: int) -> int
    // g: pointeur vers le graphe
    // r: label du sommet source

Début
    // Initialisation
    Pour chaque sommet u dans le graphe g
        u.distance = INFINI
        u.visite = FAUX

    // Trouver le sommet de départ avec le label r
    src = chercher_sommet(g, r)
    Si src est NULL
        Retourner -1  // Le sommet source n'a pas été trouvé
    Fin Si
    src.distance = 0

    // Boucle principale de l'algorithme de Dijkstra
    Pour i de 1 à nombre_sommets(g)
        // Trouver le sommet non visité avec la plus petite distance
        u = NULL
        minDist = INFINI
        Pour chaque sommet v dans le graphe g
            Si non v.visite et v.distance < minDist
                minDist = v.distance
                u = v
            Fin Si
        Fin Pour

        Si u est NULL
            Sortir de la boucle // Aucun sommet atteignable
        Fin Si
        u.visite = VRAI  // Marquer le sommet u comme visité

        // Mise à jour des distances des sommets voisins de u
        Pour chaque arc a partant de u
            alt = u.distance + a.poids
            Si alt < a.dest.distance
                a.dest.distance = alt
            Fin Si
        Fin Pour
    Fin Pour

    Retourner 0  // Succès
Fin


*/

// internet
int dijkstra(pgraphe_t g, int r) {
    psommet_t u, v;
    parc_t a;
    int alt;

    // Initialisation des sommets
    u = g;
    while (u != NULL) {
        u->distance = INT_MAX;
        u->visite = 0;
        u = u->sommet_suivant;
    }

    // Trouver le sommet de départ
    psommet_t src = chercher_sommet(g, r);
    if (src == NULL) return -1; // Sommet source non trouvé
    src->distance = 0;

    // Implémentation de l'algorithme de Dijkstra
    for (int i = 0; i < nombre_sommets(g); i++) {
        // Sélection du sommet avec la distance minimale
        int minDist = INT_MAX;
        u = NULL;
        v = g;
        while (v != NULL) {
            if (!v->visite && v->distance < minDist) {
                minDist = v->distance;
                u = v;
            }
            v = v->sommet_suivant;
        }

        if (u == NULL) break; // Aucun sommet atteignable
        u->visite = 1; // Marquer comme visité

        // Mise à jour des distances
        a = u->liste_arcs;
        while (a != NULL) {
            alt = u->distance + a->poids;
            if (alt < a->dest->distance) {
                a->dest->distance = alt;
            }
            a = a->arc_suivant;
        }
    }

    return 0; // Succès
}




/* PSEUDO-CODE PIR LA FCT elementaire : 


Fonction elementaire(g: pgraphe_t, c: chemin_t) -> int
    Début
        Si c est NULL ou c.sommet_courrant est NULL
            Retourner 1  // Un chemin vide ou avec un seul sommet est élémentaire

        Initialiser un tableau visites de taille g.nombre_sommets à 0

        Définir current comme pointeur sur c  // Commencer avec le premier élément du chemin
        isElementary à 1  // Supposer que le chemin est élémentaire

        Tant que current n'est pas NULL et current.sommet_courrant n'est pas NULL
            index = current.sommet_courrant.label  // Utiliser le label du sommet comme index

            Si visites[index] est 1
                isElementary à 0  // Chemin non élémentaire, car sommet déjà visité
                Sortir de la boucle

            Mettre visites[index] à 1  // Marquer le sommet comme visité
            current = current.sommet_suivant  // Passer au sommet suivant dans le chemin

        Libérer le tableau visites
        Retourner isElementary  // Retourner le statut du chemin (élémentaire ou non)

    Fin
*/