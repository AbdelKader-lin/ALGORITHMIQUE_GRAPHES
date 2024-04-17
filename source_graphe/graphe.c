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
      pos_courante->sommet_courrant->visite = 1 ;
      parc_t arc_cour = pos_courante_1->sommet_courrant->liste_arcs ; 
      while ( arc_cour != NULL ){
        if ( arc_cour->arc_visite == 0 ){
          arc_cour->arc_visite = 1 ;
        }
        arc_cour = arc_cour->arc_suivant ; 
      }
    }
    pos_courante = pos_courante->sommet_suivant ;
  }

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
        return 0 ; // Il y a un sommets qui n'est pas présent dans le chemin
      }
    pos_courante_2 = pos_courante_2->sommet_suivant ;
  }

  return 1 ; 
}

/*

Un graphe est dit Eulérien si il existe au moins un chemin qui soit Eulérien.


Décrivez en C l’implémentation de la fonction graphe_eulerien qui vérifie si un graphe est Eulérien ou pas. 

La fonction graphe_eulerien renvoie 1 si le graphe g est Eulérien, 0 sinon.

*/
int graphe_eulerien ( pgraphe_t g ){
//
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
  pgraphe_t sommet_X = NULL ;
  pgraphe_t sommet_Y = NULL ;

  psommet_t sommet_courrant = g ;
  while ( sommet_courrant != NULL ){
    // somme de label = x trouvé
    if ( sommet_courrant->label == x ){
      sommet_X = sommet_courrant ;
    }
    // somme de label = y trouvé
    if ( sommet_courrant->label == y ){
      sommet_Y = sommet_courrant ;
    }
    sommet_courrant = sommet_courrant->sommet_suivant ;
  }

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
