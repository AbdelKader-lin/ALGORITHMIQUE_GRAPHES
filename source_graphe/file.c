#include <stdio.h>
#include <stdlib.h>

//#include "abr.h"
#include "file.h"


/*
typedef struct {
  int tete ;
  int queue ;
  pnoeud_t Tab [MAX_FILE_SIZE] ;
} file_t, *pfile_t ;
*/


pfile_t creer_file ()
{
   pfile_t PtrNouvFile = ( pfile_t ) malloc( sizeof( file_t ) ) ;
  if ( PtrNouvFile == NULL ){
    printf("Erreur d'allocation de mémoire pour la file !\n");
    exit(1);
  }
  PtrNouvFile->tete = -1 ; 
  PtrNouvFile->queue = -1 ;
  return PtrNouvFile ;
}

int detruire_file (pfile_t f){
  if (f == NULL) {
    return -1; // La file n'existe pas, donc on ne peut pas la détruire.
  }

  free(f); // Libère la mémoire allouée pour la file elle-même.
  return 0; // Opération réussie.
}




int file_vide (pfile_t f)
{
  return ( f->tete == -1 ) ;
}

int file_pleine (pfile_t f)
  {
    return ( f->queue == MAX_FILE_SIZE - 1 );
}

pnoeud_t defiler (pfile_t f){
  if (file_vide(f)) {
        printf("Erreur: la file est vide.\n");
        return NULL;
    }

    pnoeud_t elt = f->Tab[f->tete];
    if (f->tete == f->queue) { // File formé d'un seul elt
        // File devient alors vide
        f->tete = -1;
        f->queue = -1;
    } else {
        f->tete++ ;
    }
    return elt;
}

int enfiler (pfile_t f, pnoeud_t p){
  if (file_pleine(f)) {
    printf("Erreur: la file est pleine.\n");
    return -1;
  }

  if (file_vide(f)) {
    f->tete = 0;
  }
  f->queue++ ;
  f->Tab[ f->queue ] = p ; 
  return 0;
}