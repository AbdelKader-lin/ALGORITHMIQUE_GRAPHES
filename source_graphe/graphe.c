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

#define Entier_max 2147483647 // 2^31 -1

// On defini un type chemin : une liste chainée de sommets 
typedef struct chemin {
    psommet_t sommet_courrant ;
    psommet_t sommet_suivant ;  
} chemin_t;



psommet_t chercher_sommet (pgraphe_t g, int label)
{
  psommet_t s ;

  s = g ;

  while ((s!= NULL) && (s->label != label))
    {
      s = s->sommet_suivant ;
    }
  return s ;
}

parc_t existence_arc (parc_t l, psommet_t s)
{
  parc_t p = l ;

  while (p != NULL)
    {
      if (p->dest == s)
	return p ;
      p = p->arc_suivant ;
    }
  return p ;
  
}


void ajouter_arc (psommet_t o, psommet_t d, int distance)
{
  parc_t parc ;

  parc = (parc_t) malloc (sizeof(arc_t)) ;

  if (existence_arc (o->liste_arcs, d) != NULL)
    {
      fprintf(stderr, "ajout d'un arc deja existant\n") ;
      exit (-1) ;
    }
  
  parc->poids = distance ;
  parc->dest = d ;
  parc->arc_suivant = o->liste_arcs ;
  o->liste_arcs = parc ;
  return ;
}



// ===================================================================

int nombre_sommets (pgraphe_t g)
{
  psommet_t p = g ;
  int nb = 0 ;

  while (p != NULL)
    {
      nb = nb + 1 ;
      p = p->sommet_suivant ;
    }

  return nb ;
}

int nombre_arcs (pgraphe_t g)
{

  psommet_t p = g ;
  int nb_arcs = 0 ;

  while (p != NULL)
    {
      parc_t l = p->liste_arcs ;

      while (l != NULL)
	{
          nb_arcs = nb_arcs + 1 ;
	  l = l->arc_suivant ;
	}
      
      p = p->sommet_suivant ;
    }
  return nb_arcs ;
}

void init_couleur_sommet (pgraphe_t g)
{
  psommet_t p = g ;

  while (p != NULL)
    {
      p->couleur = 0 ; // couleur indefinie
      p = p->sommet_suivant ; // passer au sommet suivant dans le graphe
    }
  
  return ;
}

int colorier_graphe (pgraphe_t g)
{
  /*
    coloriage du graphe g
    
    datasets
    graphe data/gr_planning
    graphe data/gr_sched1
    graphe data/gr_sched2
  */

  psommet_t p = g ;
  parc_t a ;
  int couleur ;
  int max_couleur = INT_MIN ; // -INFINI
  
  int change ;

  init_couleur_sommet (g) ;
  
  while (p != NULL)
    {
      couleur = 1 ; // 1 est la premiere couleur

      // Pour chaque sommet, on essaie de lui affecter la plus petite couleur

      // Choix de la couleur pour le sommet p
      
      do
	{
	  a = p->liste_arcs ;
	  change = 0 ;
      
	  while (a != NULL)
	    {
	      if (a->dest->couleur == couleur)
		{
		  couleur = couleur + 1 ;
		  change = 1 ;
		} 
	      a = a->arc_suivant ; 
	    }

	} while (change == 1) ;

      // couleur du sommet est differente des couleurs de tous les voisins
      
      p->couleur = couleur ;
      if (couleur > max_couleur)
	max_couleur = couleur ;

      p = p->sommet_suivant ;
    }
  
  return max_couleur ;
}


/*
Les types utilisés :
-------------------------

typedef struct a *parc_t ;


un graphe est constitué d'une liste de sommets


typedef struct s
{
  int        label ; 
  parc_t     liste_arcs ; 
  struct s   *sommet_suivant ; 
  int        couleur ; 
} sommet_t, *psommet_t ;


Les arcs sortants d'un sommet sont chainés
Pour chaque arc, il y a un poids qui peut par exemple correspondre a une distance


typedef struct a {
  
                  int         poids ;
                  psommet_t   dest ;  
                  struct a *  arc_suivant ; 
  
} arc_t, *parc_t ;


pgraphe_t: pointeur vers le premier sommet d'un graphe

typedef psommet_t pgraphe_t ;
*/




// afficher les sommets du graphe avec un parcours en largeur
void afficher_graphe_largeur(pgraphe_t g, int r) {
    

    // 1- Verifier si le graphe est vide
    if ( g == NULL ){
      printf("Le graphe est vide, parcours impossible !");
      return ; 
    }
    
    // 2- On remet le champ visite à 0 pour tous les sommets du graphe 
    pgraphe_t noeud_temp = g ; 
    while ( noeud_temp != NULL ){
      noeud_temp->visite = 0 ; 
      noeud_temp = noeud_temp->sommet_suivant ;
    }



    // 3- Recherche du sommet de départ
    pgraphe_t sommet_de_depart = g ; 
    while ( sommet_de_depart->label != r ){
      sommet_de_depart = sommet_de_depart->sommet_suivant ; 
    }
    if ( sommet_de_depart == NULL ){
      printf("Sommet de départ non trouvé !\n");
      return ;
    }


    // 4- File pour les sommets à visiter
    pfile_t file_de_parcours = creer_file() ;
    pnoeud_t noeud_de_depart ;
    noeud_de_depart->cle = sommet_de_depart ;  
    int enfiler ( file_de_parcours, noeud_de_depart ) ;


    // 5- Commencez le parcours du graphe par le sommet r
    pgraphe_t courant = noeud_de_depart ; 
    while ( !file_vide( file_de_parcours ) ){
      courant = defiler( file_de_parcours ) ; 
      if ( courant->visite == 0 ){
        courant->visite = 1 ;
        printf(" Sommet , couleur :  %i ; %i\n", courant->label , courant->couleur ) ;
        parc_t arc = courant->liste_arcs ;
        while ( arc != NULL ){
          if ( arc->dest->visite == 0 ) {
            enfiler( file_de_parcours , arc->dest );
          }
          arc = arc->arc_suivant ; 
        }

      }
    }

}





// afficher les sommets du graphe avec un parcours en profondeur
void afficher_graphe_profondeur (pgraphe_t g, int r){
  
  // 1- Verifier si le graphe est vide
  if ( g == NULL ){
    printf("Le graphe est vide, parcours impossible !");
    return ; 
  }
    
  // 2- On remet le champ visite à 0 pour tous les sommets du graphe 
  pgraphe_t noeud_temp = g ; 
  while ( noeud_temp != NULL ){
    noeud_temp->visite = 0 ; 
    noeud_temp = noeud_temp->sommet_suivant ;
  }



  // 3- Recherche du sommet de départ
  pgraphe_t sommet_de_depart = g ; 
  while ( sommet_de_depart->label != r ){
    sommet_de_depart = sommet_de_depart->sommet_suivant ; 
  }
  if ( sommet_de_depart == NULL ){
    printf("Sommet de départ non trouvé !\n");
    return ;
  }

  // Appel de la fonction de parcours recursive avec le sommet 
  // de départ en parametre
  parcours_largeur_rec( sommet_de_depart ) ;

}

void parcours_largeur_rec( pgraphe_t sommet ){


  // Création de la pile de parcours
  ppile_t pile_de_parcours =  creer_pile () ;
  empiler( pile_de_parcours , sommet ) ; 

  // Parcours en profondeur 
  while ( !pile_vide( pile_de_parcours ) ) {
    psommet_t tmp = depiler( pile_de_parcours ) ;
    if ( tmp->visite == 0 ){
      tmp->visite = 1 ; 
      printf(" Sommet , couleur :  %i ; %i\n", tmp->label , tmp->couleur ) ;
      parc_t arc = tmp->liste_arcs ;
      while ( arc != NULL ){
        if ( arc->dest->visite == 0 ) {
          empiler( pile_de_parcours , arc->dest );
        }
        arc = arc->arc_suivant ; 
      }
    } 
  }
}


/*
Algorithme de dijkstra. des
variables ou des chanmps doivent 
etre ajoutees dans les structures.
*/
void algo_dijkstra (pgraphe_t g, int r){

  // 1- Verifier si le graphe est vide
  if ( g == NULL ){
    printf("Le graphe est vide, parcours impossible !");
    return ; 
  }


    // 2- Initialiser les distances et les visites pour tous les sommets
    psommet_t sommet_actuel = g ;
    while ( sommet_actuel != NULL ){
      sommet_actuel->distance = INT_MAX;
      sommet_actuel->visite = 0;
      sommet_actuel = sommet_actuel->sommet_suivant ; 
    }

    // 3- On trouve le sommet de départ du graphe
    psommet_t sommet_depart = NULL ;
    psommet_t sommet_courant = NULL ;

    while ( sommet_courant != NULL ){
      if ( sommet_courant ->label == r ){
        sommet_depart = sommet_courant ;
        break ;
      }
      sommet_courant = sommet_courant->sommet_suivant ; 
    }
    if ( sommet_depart == NULL ){
      printf(" SOMMET DE DÉPART NON TROUVÉ ! \n");
      return ;
    } else {
      // distance du sommet de départ à lui meme tjrs = 0
      sommet_depart->distance = 0 ; 
    }



    psommet_t courrant_sommet = sommet_depart ; 

    // Boucle principale
    while ( courrant_sommet != NULL ){

      // On cherche le sommet visité avec la distance minimal du sommet
      psommet_t sommet_dmin_nonvisite = NULL ; 
      psommet_t c = courrant_sommet ; 
      int dist_min =  INT_MAX ;
      while ( c != NULL ){
        if ( c->visite == 0 && c->distance < dist_min ){
          dist_min = c->distance ; 
          sommet_dmin_nonvisite =  c ;
        }
        c = c->sommet_suivant ; 
      }

      // Plus aucun sommet à visiter, on sort de la boucle
      if ( sommet_dmin_nonvisite == NULL ){
        break ;
      }

      // Le sommet trouvé est désormais "visité"
      sommet_dmin_nonvisite->visite = 1 ;

      // On met à jour les distances 
      parc_t arc_courrant = sommet_dmin_nonvisite->liste_arcs ;
      while ( arc_courrant != NULL ){
        int d_tmp = sommet_dmin_nonvisite->distance + arc_courrant->poids ;
        if ( d_tmp < arc_courrant->dest->distance ){
          arc_courrant->dest->distance = d_tmp ;
        } 
        arc_courrant = arc_courrant->arc_suivant ;
      }

    }
}


// #############################################
// #############################################


/**
 * Vérifie si un chemin est élémentaire ou pas
 * @param g
 * @param c
 * @return 1 si le chemin est élémentaire
 *         0 sinon
 */
int elementaire ( pgraphe_t g , chemin_t c ){

}




// ======================================================================




int degre_sortant_sommet (pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs sortants 
    du sommet n dans le graphe g
  */ 

  return 0 ;
}

int degre_entrant_sommet (pgraphe_t g, psommet_t s)
{
  /*
    Cette fonction retourne le nombre d'arcs entrants 
    dans le noeud n dans le graphe g
  */ 

  return 0 ;
}

int degre_maximal_graphe (pgraphe_t g)
{
  /*
    Max des degres des sommets du graphe g
  */

  return 0 ;
}


int degre_minimal_graphe (pgraphe_t g)
{
  /*
    Min des degres des sommets du graphe g
  */

  return 0 ;
}


int independant (pgraphe_t g)
{
  /* Les aretes du graphe n'ont pas de sommet en commun */

  return 0 ;
}



int complet (pgraphe_t g)
{
  /* Toutes les paires de sommet du graphe sont jointes par un arc */

  return 0 ;
}

int regulier (pgraphe_t g)
{
  /* 
     graphe regulier: tous les sommets ont le meme degre
     g est le ponteur vers le premier sommet du graphe
     renvoie 1 si le graphe est régulier, 0 sinon
  */

  return 0 ;
}




/*
  placer les fonctions de l'examen 2017 juste apres
*/
