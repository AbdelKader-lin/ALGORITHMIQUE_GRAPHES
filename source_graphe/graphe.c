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

#define Entier_max 2147483647 // 2^31 -1






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

void afficher_graphe_largeur(pgraphe_t g, int r) {
    

    // On vérifie si le graphe est vide 
    if ( g == NULL ){
      printf("Le graphe est vide, parcours impossible !");
      return ; 
    }

    pfile_t FilePrincipal = creer_file() ;

    pnoeud_t noeud_init ;
    psommet_t somme_depart = chercher_sommet( g, r ) ; 

    int eval_enfiler_1 =  enfiler ( FilePrincipal , somme_depart ) ;

    while ( file_vide( FilePrincipal ) == 0 ){
      psommet_t sommet_courrant = defiler( FilePrincipal ) ;
      if ( sommet_courrant->visite == 0 ){
        sommet_courrant->visite = 1;
        printf("%i , ", sommet_courrant->label ) ; 
        parc_t arc_courrant = sommet_courrant->liste_arcs ;
        while ( arc_courrant != NULL ){
          if ( arc_courrant->dest->visite == 0 ){
            int eval_enfiler_2 =  enfiler ( FilePrincipal , arc_courrant->dest ) ;
          }
          arc_courrant = arc_courrant->arc_suivant ;
        }
      }
    }


}





// afficher les sommets du graphe avec un parcours en profondeur
void afficher_graphe_profondeur(pgraphe_t g, int r){
  
  //  On vérifie si le graphe est vide
  if ( g == NULL ){
    return ; 
  }

  // On affiche le label du sommet que l'on parcours 
  printf("%i , ", g->label ) ; 



  parc_t arc_courrant = g->liste_arcs ; 
  while ( arc_courrant != NULL ) {
    if ( arc_courrant->dest->visite == 0 ){
      arc_courrant->dest->visite = 1 ; 
      afficher_graphe_profondeur( arc_courrant->dest , arc_courrant->dest->label ) ; 
    }
    arc_courrant = arc_courrant->arc_suivant ; 
  }
  return ;
}



void algo_dijkstra (pgraphe_t g, int r){

  // On verifier si le graphe est vide
  if ( g == NULL ){
    printf("Le graphe est vide, parcours impossible !");
    return ; 
  }


    // On initialiser les distances et les visites pour tous les sommets du graphe
    psommet_t sommet_actuel = g ;
    while ( sommet_actuel != NULL ){
      sommet_actuel->distance = INT_MAX;
      sommet_actuel->visite = 0;
      sommet_actuel = sommet_actuel->sommet_suivant ; 
    }

    // On cherche le sommet de départ : le sommet dont le label = r
    psommet_t sommet_depart = chercher_sommet( g , r ) ;
    // d du sommet de déepart à lui meme est 0
    sommet_depart->distance = 0 ;  
    sommet_depart->visite = 1 ; 

    // On entre dans la boucle principal 
    while ( sommet_depart != NULL ){
      // On a déja visité le sommet précedent
      // On cherche donc le prochain sommet à visiter : sommet à d minimal
      int d_min = INT_MAX ;
      psommet_t sommet_d_min = NULL ; 
      parc_t arc_courrant = sommet_depart->liste_arcs ; 

      // On parcours les sommets destinations des arcs sortant de sommet départ
      while( arc_courrant != NULL ){
        // On regarde si on peut faire mieux en terme de la distance du sommet destination
        if ( sommet_depart->distance + arc_courrant->poids < arc_courrant->dest->distance ){
          arc_courrant->dest->distance = sommet_depart->distance + arc_courrant->poids ;
        }

        // Si le sommet destination de cet arc n'a pas encore été visité
        if ( arc_courrant->dest->visite == 0 ){
          // Le sommet destination de cet arc est desormais visité
          arc_courrant->dest->visite == 1 ;
         
          if ( arc_courrant->dest->distance < d_min ){
            d_min = arc_courrant->dest->distance + arc_courrant->poids ;
            sommet_d_min = arc_courrant->dest ; 
          }
 
        }
        arc_courrant = arc_courrant->arc_suivant ;
      }
      sommet_depart = sommet_d_min ;
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
int elementaire ( pgraphe_t graphe , pchemin_t chemin ){

  if ( chemin == NULL || chemin->sommet_courrant == NULL ){
    // C'est un chemin vide, ou bien formé d'un seul sommet
    return 1 ; 
  }

  // On initialise le champs visite à 0 pour tout les sommets;
  pchemin_t pos_courante_1 = chemin ;
  while ( pos_courante_1 != NULL ){
    pos_courante_1 ->sommet_courrant ->visite = 0  ;
    pos_courante_1 = pos_courante_1->sommet_suivant ;
  }

  // On parcours les sommets du chemin
  pchemin_t pos_courante = chemin ;
  while ( pos_courante != NULL ){
    if (  pos_courante->sommet_courrant->visite == 1 ){
      return 0 ;
    }
    pos_courante ->sommet_courrant ->visite = 1  ;
    pos_courante = pos_courante->sommet_suivant ;
  }
  return 1 ; // Le chemin est elementaire 
}




/*

Un chemin simple est un chemin ne passant pas deux fois par le même arc, c’est à dire
un chemin dont tous les arcs sont distincts.

Décrivez en C l’implémentation de la fonction simple qui vérifie si un chemin est simple
ou pas. La fonction simple renvoie 1 si le chemin c est simple, 0 sinon.

*/
int simple ( pgraphe_t g , pchemin_t chemin ) {
  if ( chemin == NULL || chemin->sommet_courrant == NULL ){
    // C'est un chemin vide, ou bien formé d'un seul 
    return 1 ; 
  }

  // On initialise le champs visite à 0 pour tout les sommets et tous les arcs ;
  pchemin_t pos_courante_1 = chemin ;
  while ( pos_courante_1 != NULL ){
    pos_courante_1 ->sommet_courrant ->visite = 0  ;
    parc_t arc_c = pos_courante_1->sommet_courrant->liste_arcs ; 
    while ( arc_c != NULL ){
      arc_c->arc_visite = 0 ;
      arc_c = arc_c->arc_suivant ;
    }
    pos_courante_1 = pos_courante_1->sommet_suivant ;
  }

  // On parcours les arcs du chemin
  pchemin_t pos_courante = chemin ;
  while ( pos_courante != NULL ){
    if (  pos_courante->sommet_courrant->visite == 0 ){
      parc_t arc_cour = pos_courante_1->sommet_courrant->liste_arcs ; 
      while ( arc_cour != NULL ){
        if ( arc_cour->arc_visite == 1 ){
          return 0 ;
        }
        arc_cour->arc_visite = 1 ;
        arc_cour = arc_cour->arc_suivant ; 
      }
    }
    pos_courante = pos_courante->sommet_suivant ;
  }
  return 1 ; // Le chemin est elementaire 
}


/*
Un chemin est dit Eulérien si tous les arcs du graphe sont utilisés dans le chemin.


Décrivez en C l’implémentation de la fonction eulerien qui vérifie si un chemin est Eulérien ou pas.
La fonction eulerien renvoie 1 si le chemin c est Eulérien, 0 sinon.

*/

int eulerien ( pgraphe_t g , pchemin_t chemin ) {
  if ( chemin == NULL || chemin->sommet_courrant == NULL ){
    // C'est un chemin vide, ou bien formé d'un seul 
    return 0 ; 
  }

  // On initialise le champs visite à 0 pour tout les sommets et tous les arcs ;
  pchemin_t pos_courante_1 = chemin ;
  while ( pos_courante_1 != NULL ){
    pos_courante_1 ->sommet_courrant ->visite = 0  ;
    parc_t arc_c = pos_courante_1->sommet_courrant->liste_arcs ; 
    while ( arc_c != NULL ){
      arc_c->arc_visite = 0 ;
      arc_c = arc_c->arc_suivant ;
    }
    pos_courante_1 = pos_courante_1->sommet_suivant ;
  }

  // On parcours les arcs du chemin pour les marquer
  pchemin_t pos_courante = chemin ;
  while ( pos_courante != NULL ){
    if (  pos_courante->sommet_courrant->visite == 0 ){
      pos_courante->sommet_courrant->visite = 1 ;

      // On parcours la liste des arcs sortant de pos_courante->sommet_courrant
      parc_t arc_cour = pos_courante_1->sommet_courrant->liste_arcs ; 
      while ( arc_cour != NULL ){
        // Arc non visité
        if ( arc_cour->arc_visite == 0 ){
          arc_cour->arc_visite = 1 ;
        }
        arc_cour = arc_cour->arc_suivant ; 
      }
    }
    pos_courante = pos_courante->sommet_suivant ;
  }

  // Maintenant on parcours tous les arcs du graphe,
  // Si un de ces arcs n'est pas visité, c'est qu'il n'est pas dans le chemin
  pgraphe_t pos_courante_2 = g ;
  while ( pos_courante_2 != NULL ){
    parc_t arc_c = pos_courante_2->liste_arcs ; 
    while ( arc_c != NULL ){
      if ( arc_c->arc_visite == 0 ){
        return 0 ; // Il y a un arc qui n'est pas présent dans le chemin
      }
      arc_c = arc_c->arc_suivant ;
    }
    pos_courante_2 = pos_courante_2->sommet_suivant ;
  }

  return 1 ; 
}


/*
Un chemin est dit Hamiltonien si tous les sommets du graphe sont utilisés dans le chemin.

Décrivez en C l’implémentation de la fonction hamiltonien qui vérifie si un chemin est Hamiltonien ou pas. 

La fonction hamiltonien renvoie 1 si le chemin c est Hamiltonien, 0sinon.

*/

int hamiltonien ( pgraphe_t g , pchemin_t chemin ){
  if ( chemin == NULL || chemin->sommet_courrant == NULL ){
    // C'est un chemin vide, ou bien formé d'un seul 
    return 1 ; 
  }

  // On initialise le champs visite à 0 pour tout les sommets et tous les arcs ;
  pchemin_t pos_courante_1 = chemin ;
  while ( pos_courante_1 != NULL ){
    pos_courante_1 ->sommet_courrant ->visite = 0  ;
    parc_t arc_c = pos_courante_1->sommet_courrant->liste_arcs ; 
    while ( arc_c != NULL ){
      arc_c->arc_visite = 0 ;
      arc_c = arc_c->arc_suivant ;
    }
    pos_courante_1 = pos_courante_1->sommet_suivant ;
  }

  // On parcours les sommets du chemin
  pchemin_t pos_courante = chemin ;
  while ( pos_courante != NULL ){
    if (  pos_courante->sommet_courrant->visite == 0 ){
      pos_courante->sommet_courrant->visite = 1 ;
    }
    pos_courante = pos_courante->sommet_suivant ;
  }

  pgraphe_t pos_courante_2 = g ;
  while ( pos_courante_2 != NULL ){
    if ( pos_courante_2->visite == 0 ){
        return 0 ; // Il y a un sommet qui n'est pas présent dans le chemin
      }
    pos_courante_2 = pos_courante_2->sommet_suivant ;
  }

  return 1 ; 
}


/* 
Décrivez en C l’implémentation de la fonction graphe_hamiltonien qui vérifie si un graphe
est Hamiltonien ou pas. La fonction graphe_hamiltonien renvoie 1 si le graphe g est Ha-
miltonien, 0 sinon.
*/
int graphe_hamiltonien ( pgraphe_t g ){ 
  psommet_t sommet_courrant_1 = g ;
  while ( sommet_courrant_1 != NULL ){
    // On parcours de nouveau tout les sommets et on genere un chemin entre sommet_courrant_1 et chacun de ces sommets
    psommet_t sommet_courrant_2 = g ; 
    while ( sommet_courrant_2 != NULL ){
      pchemin_t MonChemin = chemin( g , sommet_courrant_1 , sommet_courrant_2 );
      // Je vérifie si le chemin genere est hamiltonien ou pas
      int eval_chemin = hamiltonien( g , MonChemin ) ;
      //  Si le chemin genere est hamiltonien, on renvoie 1
      if ( eval_chemin == 1 ){
        return 1 ; 
      }
      sommet_courrant_2 = sommet_courrant_2->sommet_suivant ;
    }
    sommet_courrant_1 = sommet_courrant_1->sommet_suivant ; 
  }
  // Aucun chemin n'est pas hamiltonien
  return 0 ;
}

/*

Un graphe est dit Eulérien si il existe au moins un chemin qui soit Eulérien.


Décrivez en C l’implémentation de la fonction graphe_eulerien qui vérifie si un graphe est Eulérien ou pas. 

La fonction graphe_eulerien renvoie 1 si le graphe g est Eulérien, 0 sinon.

*/
int graphe_eulerien ( pgraphe_t g ){
  psommet_t sommet_courrant_1 = g ;
  while ( sommet_courrant_1 != NULL ){
    // On parcours de nouveau tout les sommets et on genere un chemin entre sommet_courrant_1 et chacun de ces sommets
    psommet_t sommet_courrant_2 = g ; 
    while ( sommet_courrant_2 != NULL ){
      pchemin_t MonChemin = chemin( g , sommet_courrant_1 , sommet_courrant_2 );
      // Je vérifie si le chemin genere est eulerien ou pas
      int eval_chemin = eulerien( g , MonChemin ) ;
      //  Si le chemin genere est eulerien, on renvoie 1
      if ( eval_chemin == 1 ){
        return 1 ; 
      }
      sommet_courrant_2 = sommet_courrant_2->sommet_suivant ;
    }
    sommet_courrant_1 = sommet_courrant_1->sommet_suivant ; 
  }
  // Aucun chemin n'est pas eulerien
  return 0 ;
}


/*
La longueur d’un chemin est la somme des poids des arcs.
La distance entre deux sommets x et y est la longueur du plus court chemin entre x et y.
*/

/*
Décrivez en C l’implémentation de la fonction distance qui calcule la distance entre deux
sommets avec les labels x et y dans le graphe g.
*/
int distance ( pgraphe_t g , int x , int y ){
  // On veut tout d'abord trouver les sommet avec le label x et y 
  pgraphe_t sommet_X = chercher_sommet( g , x ) ;
  pgraphe_t sommet_Y = chercher_sommet( g , y ) ;

  /*
  typedef struct chemin {
    psommet_t sommet_courrant ;
    psommet_t sommet_suivant ;
  } chemin_t , *pchemin_t ;
  */

  int distance = 0 ;  
  pgraphe_t current_sommet = sommet_X ;
  while ( sommet_X != NULL  && sommet_X != sommet_Y ){
    if ( current_sommet->visite == 0 ){
      current_sommet->visite = 1 ; // Le sommet est desormais visité

      // On trouve l'arc avec le poids le plus petit
      parc_t arc_courrant = current_sommet->liste_arcs ;
      if ( arc_courrant == NULL ){
        printf(" Sommet Y non trouvé !");
        return -1 ; 
      }
      // On parcours les arcs sortant du sommet que l'on est en train de visiter
      parc_t arc_poids_min = arc_courrant ; 
      int d_min = INT_MAX ;
      while ( arc_courrant != NULL ){
        if ( arc_courrant->poids < d_min ) {
          d_min = arc_courrant->poids ;
          arc_poids_min = arc_courrant ; 
        }
        arc_courrant = arc_courrant->arc_suivant ; 
      }
      distance += d_min ;
      current_sommet = arc_poids_min->dest ; 
    } else{
      current_sommet = current_sommet->sommet_suivant ; 
    }

  }
  return distance ; 
}
    


/*
L’excentricité d’un sommet est sa distance maximale avec les autres sommets du graphe

Le diamètre d’un graphe est l’excentricité maximale de ses sommets



Décrivez en C l’implémentation de la fonction diametre qui calcule le diamètre du graphe g.
*/
int diametre ( pgraphe_t g ){
  int diametre = INT_MIN ; 

  psommet_t sommet_courrant = g ; 
  // On calcule pour chaque sommet, sa distance max avec les autres sommets
  while ( sommet_courrant != NULL ){
    int mon_extrencite = excentricite( g , sommet_courrant->label ) ;
    if ( mon_extrencite > diametre ){
      diametre = mon_extrencite ; 
    }
    sommet_courrant = sommet_courrant->sommet_suivant ;
  }
  return diametre ;

}


int excentricite( pgraphe_t g , int n ) {
  // On trouve d'abord le sommet dont le label = n 
  psommet_t mon_sommet = chercher_sommet ( g, n ) ; 

  psommet_t sommet_courrant = g ; 
  int d_max = INT_MIN ; 
  while ( sommet_courrant != NULL ){
    int d = distance( g , sommet_courrant , mon_sommet ); 
    if ( d > d_max ){
      d_max = d ; 
    }
    sommet_courrant = sommet_courrant->sommet_suivant ; 
  }
  return d_max ; 
}




/*
La fonction suivante renvoie le meilleur chemin entre le sommet de label x et le sommet de label y
*/

pchemin_t chemin ( pgraphe_t g , int x , int y ){

  // On veut tout d'abord trouver les sommet avec le label x et y 
  pgraphe_t sommet_X = chercher_sommet( g , x ) ;
  pgraphe_t sommet_Y = chercher_sommet( g , y ) ;


  // On va utiliser l'algo de Dijkstra pour determiner le meilleur chemin entre x et y 

  // On determine la distance de chaque sommet de g par rapport au sommet de label X
  algo_dijkstra( g , sommet_X->label ) ;

  pchemin_t mon_chemin ; 
  mon_chemin->sommet_courrant = sommet_X ;
  mon_chemin->sommet_suivant = NULL ;

  pchemin_t chemin_courr = mon_chemin ; 

  psommet_t sommet_courrant_1 = g ; 
  while ( sommet_courrant_1 != NULL ){
    parc_t arc_courrant = sommet_courrant_1->liste_arcs ; 
    psommet_t sommet_d_min = sommet_courrant_1 ; 
    int dist_min = INT_MAX ;
    while ( arc_courrant != NULL ){
      if ( arc_courrant->dest->distance < dist_min ){
        dist_min = arc_courrant->dest->distance ; 
        sommet_d_min = sommet_courrant_1 ;
      }
      arc_courrant = arc_courrant->arc_suivant ;
    }
  chemin_courr->sommet_suivant = sommet_d_min ; 
  sommet_courrant_1 = sommet_courrant_1->sommet_suivant ; 
  }

  return mon_chemin ; 
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
