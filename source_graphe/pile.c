#include <stdlib.h>
#include <stdio.h>
#include "abr.h"
#include "pile.h"



/*
typedef struct {
  int sommet ;
  pnoeud_t Tab [MAX_PILE_SIZE] ;
} pile_t, *ppile_t ;
*/


ppile_t creer_pile ()
{
  ppile_t PtrNouvPile = ( ppile_t ) malloc( sizeof( pile_t ) ) ;
  if ( PtrNouvPile == NULL ){
    printf("Erreur d'allocation de mémoire !\n");
    exit(1);
  }
  PtrNouvPile->sommet = -1 ; 
  return PtrNouvPile ;
}

int detruire_pile (ppile_t p)
{
  if ( p == NULL ){
    printf("La pile est null !\n") ;
    return -1 ;
  }
  free( p );
  return 1 ;
}  

int pile_vide (ppile_t p)
{
  return (p != NULL && p->sommet == 0);
}

int pile_pleine (ppile_t p)
 {

  if ( p != NULL  && p ->sommet == MAX_PILE_SIZE - 1 ){
    return 1 ;
  }

  return 0 ;
} 

pnoeud_t depiler (ppile_t p)
{
  pnoeud_t r = NULL;
  if (p != NULL && p->sommet > 0) {
    p->sommet--;
    return p->Tab[p->sommet];
  }
  return r;
  /*
     a completer
  

 if (p == NULL || p->sommet < 0) {
        return NULL; // La pile est vide ou n'existe pas
  }

 pnoeud_t NouvNoeudPtr = (pnoeud_t)malloc(sizeof(noeud_t));
 
 NouvNoeudPtr->cle = p->Tab[ p->sommet ]->cle ; 
 NouvNoeudPtr->fdroite = p->Tab[ p->sommet ]->fdroite ; 
 NouvNoeudPtr->fgauche = p->Tab[ p->sommet ]->fgauche ; 
 

 free( p->Tab[ p->sommet ] );
 p->sommet--;


  return NouvNoeudPtr ;*/

}

int empiler (ppile_t p, pnoeud_t pn)
  {
  /*
     a completer
  */

 if ( p == NULL  || pile_pleine( p ) ){
  return -1 ;
 } else {
  p->Tab[ p->sommet + 1 ] = pn ; 
  p->sommet++;
  return 0 ;
 }

}
