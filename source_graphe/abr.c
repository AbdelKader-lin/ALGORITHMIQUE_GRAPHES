#include <stdio.h>
#include <stdlib.h>

#include "abr.h"
#include "pile.h"
#include "file.h"


#define max(a,b) ((a)>(b)?(a):(b))


int feuille (Arbre_t a)
{
  if (a == NULL)
    return 0 ;
  else
    {
      if ((a->fgauche == NULL) && (a->fdroite == NULL))
	return 1 ;
      else
	return 0 ;
    }
}

Arbre_t ajouter_noeud (Arbre_t a, Arbre_t n)
{
  /* ajouter le noeud n dans l'arbre a */
  
  if (a == NULL)
    return n ;
  else if (n->cle < a->cle)
	a->fgauche = ajouter_noeud (a->fgauche, n) ;
  else
	a->fdroite = ajouter_noeud (a->fdroite, n) ;
  return a ;
  
}  

Arbre_t rechercher_cle_arbre (Arbre_t a, int valeur)
{
  if (a == NULL)
    return NULL ;
  else
    {
      if (a->cle == valeur)
	return a ;
      else
	{
	  if (a->cle < valeur)
	    return rechercher_cle_arbre (a->fdroite, valeur) ;
	  else
	    return rechercher_cle_arbre (a->fgauche, valeur) ;
	}
    }
}

Arbre_t ajouter_cle (Arbre_t a, int cle)
{
  Arbre_t n ;
  Arbre_t ptrouve ;
  
  /* 
     ajout de la clé. Creation du noeud n qu'on insere 
    dans l'arbre a
  */

  ptrouve = rechercher_cle_arbre (a, cle) ;

  if (ptrouve == NULL)
    {
      n = (Arbre_t) malloc (sizeof(noeud_t)) ;
      n->cle = cle;
      n->fgauche = NULL ;
      n->fdroite = NULL ;

      a = ajouter_noeud (a, n) ;
      return a ;
    }
  else
    return a ;
}


Arbre_t lire_arbre (char *nom_fichier)
{
  FILE *f ;
  int cle;
  Arbre_t a = NULL;
  
  f = fopen (nom_fichier, "r") ;

  while (fscanf (f, "%d", &cle) != EOF)
    {
      a = ajouter_cle (a, cle) ;
    }
    
  fclose (f) ;

  return a ;
}

void afficher_arbre (Arbre_t a, int niveau)
{
  /*
    affichage de l'arbre a
    on l'affiche en le penchant sur sa gauche
    la partie droite (haute) se retrouve en l'air
  */
  
  int i ;
  
  if (a != NULL)
      {
	afficher_arbre (a->fdroite,niveau+1) ;
	
	for (i = 0; i < niveau; i++)
	  printf ("\t") ;
	printf (" %d (%d)\n\n", a->cle, niveau) ;

	afficher_arbre (a->fgauche, niveau+1) ;
      }
  return ;
}


int hauteur_arbre_r (Arbre_t a){
  if ( a == NULL ){
    return 0;
  } else {
    int haut_agauche = hauteur_arbre_r( a->fgauche );
    int haut_adroit = hauteur_arbre_r( a->fdroite );
    if ( haut_agauche >= haut_adroit ){
      return 1 + haut_agauche ;
    } else {
      return 1 + haut_adroit; 
    }
  }
}

int hauteur_arbre_nr (Arbre_t a)
{
  if ( a == NULL ){
    printf("Rien à parcourir,\nL'arbre est vide") ;
    return ;
  }
  pfile_t MaFile = creer_file() ;
  int Enfile = enfiler( MaFile , a );

  int hauteur = 0 ;

  while ( !file_vide( MaFile ) ){
    pnoeud_t MonElt = defiler( MaFile );

    if ( MonElt->fgauche != NULL ){
      int EnfileG = enfiler( MaFile , MonElt->fgauche );
    }
    if ( MonElt->fdroite != NULL ){
      int EnfileD = enfiler( MaFile , MonElt->fdroite );
    }

    hauteur++ ;
  }
  detruire_file( MaFile );
  return hauteur ;
}


/*
    a completer
    Utiliser une file, voir cours
*/

void parcourir_arbre_largeur (Arbre_t a){
  if ( a == NULL ){
    printf("Rien à parcourir,\nL'arbre est vide") ;
    return ;
  }
  pfile_t MaFile = creer_file() ;
  int EnfileG = enfiler( MaFile , a );

  while ( !file_vide( MaFile ) ){
    pnoeud_t MonElt = defiler( MaFile );
    printf("Cle : %d \n", MonElt->cle );

    if ( MonElt->fgauche != NULL ){
      int EnfileG = enfiler( MaFile , MonElt->fgauche );
    }
    if ( MonElt->fdroite != NULL ){
      int EnfileD = enfiler( MaFile , MonElt->fdroite );
    }
  }

  detruire_file( MaFile ) ;

}

void afficher_nombre_noeuds_par_niveau (Arbre_t a)
{
  /*
    a completer
  */

  return ;
}


int nombre_cles_arbre_r (Arbre_t a)
{
  if ( a == NULL ){
    return 0 ; 
  }
  else {
    return 1 + nombre_cles_arbre_r( a->fdroite ) + nombre_cles_arbre_r( a->fgauche ) ;
  }
}

int nombre_cles_arbre_nr (Arbre_t a)
{
  if ( a == NULL ){
    printf("Rien à parcourir,\nL'arbre est vide") ;
    return ;
  }
  pfile_t MaFile = creer_file() ;
  int Enfile = enfiler( MaFile , a );

  int NbClefs = 0 ;

  while ( !file_vide( MaFile ) ){
    NbClefs++;
    pnoeud_t MonElt = defiler( MaFile );

    if ( MonElt->fgauche != NULL ){
      int EnfileG = enfiler( MaFile , MonElt->fgauche );
    }
    if ( MonElt->fdroite != NULL ){
      int EnfileD = enfiler( MaFile , MonElt->fdroite );
    }

  }

  detruire_file( MaFile );
  return NbClefs ;
}

int trouver_cle_min (Arbre_t a)
{
  /*
    a completer
  */

  return 0 ; 
}

 

void imprimer_liste_cle_triee_r (Arbre_t a){
  if ( a == NULL ){
    return ;
  } else {
    imprimer_liste_cle_triee_r( a->fgauche );
    printf("%d\n", a->cle );
    imprimer_liste_cle_triee_r( a->fdroite );
  }


}

void imprimer_liste_cle_triee_nr(Arbre_t a) {
    ppile_t pile = creer_pile();
    Arbre_t courant = a;

    while (courant != NULL || !pile_vide(pile)) {
        // Parcourir l'arbre à gauche
        while (courant != NULL) {
            empiler(pile, courant);
            courant = courant->fgauche;
        }

        // Revenir et traiter le noeud
        courant = depiler(pile);
        printf("%d ", courant->cle);

        // Passer au sous-arbre droit
        courant = courant->fdroite;
    }

    detruire_pile(pile);
}



int arbre_plein (Arbre_t a)
{
  if ( a == NULL ){
    return 1 ; 
  }
  if ( ( a->fgauche == NULL && a->fdroite == NULL )||
   ( a->fgauche != NULL && a->fdroite != NULL ) ){
    return 1 ;
  }

  if ( ( a->fgauche == NULL && a->fdroite != NULL )||
   ( a->fgauche != NULL && a->fdroite == NULL ) ){
    return 0 ;
  }

  return arbre_plein( a->fdroite ) && arbre_plein( a->fdroite ); 

}

int DistMin ( Arbre_t a ){
  if ( a == NULL ){
    return 0 ; 
  }
  int ProfG = 1 + DistMin( a->fgauche ) ;
  int ProfD = 1 + DistMin( a->fdroite ) ;
  if ( ProfD < ProfG ){
    return ProfD ;
  } else {
    return ProfG ;
  }
}

int DistMax ( Arbre_t a ){
  if ( a == NULL ){
    return 0 ; 
  }
  int ProfG = 1 + DistMax( a->fgauche ) ;
  int ProfD = 1 + DistMax( a->fdroite ) ;
  if ( ProfD < ProfG ){
    return ProfG ;
  } else {
    return ProfD ;
  }
}


int arbre_parfait (Arbre_t a)
{
  int DistMinim = DistMin ( a ) ;
  int DistMaxim = DistMax ( a ) ;
  return ( DistMinim == DistMaxim ) ; 
}



/*
Ecrivez la fonction rechercher_cle_sup_arbre
qui recherche la clé immédiatement supérieure 
au paramètre valeur
*/
Arbre_t rechercher_cle_sup_arbre (Arbre_t a, int valeur)
{
  /*Arbre_t Ncourrant = a ; 
  while ( Ncourrant != NULL ){
    if ( Ncourrant->cle == valeur ){
      return Ncourrant->fdroite->cle ; 
    } else {
      if ( Ncourrant->cle > valeur ){
        if ( Ncourrant->fgauche->cle < valeur ){
          return Ncourrant->cle ; 
        } else {
          Ncourrant = Ncourrant->fgauche ;
        }
      } 
    }
  }*/
  Arbre_t Ncourrant = a; 
  Arbre_t successeur = NULL;

  while (Ncourrant != NULL) {
    if (Ncourrant->cle > valeur) {
      successeur = Ncourrant;
      Ncourrant = Ncourrant->fgauche;
    } else {
      Ncourrant = Ncourrant->fdroite;
      }
    }
    return successeur;
}

Arbre_t rechercher_cle_inf_arbre(Arbre_t a, int valeur) {
    Arbre_t Ncourrant = a;
    Arbre_t predecesseur = NULL;

    while (Ncourrant != NULL) {
        if (Ncourrant->cle < valeur) {
            predecesseur = Ncourrant; // Ce noeud est un nouveau candidat pour être le prédécesseur
            Ncourrant = Ncourrant->fdroite; // Il pourrait y avoir un prédécesseur plus proche dans le sous-arbre droit
        } else {
            Ncourrant = Ncourrant->fgauche; // La clé recherchée, si elle existe, est dans le sous-arbre gauche
        }
    }

    return predecesseur; // Retourne le prédécesseur le plus proche trouvé, ou NULL si non trouvé
}



Arbre_t detruire_cle_arbre (Arbre_t a, int cle)
{
  /*
    a completer
  */

  return NULL ;
}



int ValDansArbre( Arbre_t a , int cle ){
  if ( a == NULL ){
    return 0 ;
  }
  if ( a->cle == cle ){
    return 1 ;
  } else {
    return ( ValDansArbre( a->fdroite , cle ) ) || ( ValDansArbre( a->fgauche , cle ) ) ; 
  }
}

/*
Ecrivez la fonction intersection_deux_arbre de deux 
arbres a1 et a2 avec les clés qui sont dans a1 et dans a2
*/
Arbre_t intersection_deux_arbres (Arbre_t a1, Arbre_t a2){
  Arbre_t MonArbre = NULL ;
  pfile_t MaFile = creer_file() ;
  int EnfileG = enfiler( MaFile , a1 );

  while ( !file_vide( MaFile ) ){
    pnoeud_t MonElt = defiler( MaFile );
    //printf("Cle : %d \n", MonElt->cle );
    if ( ValDansArbre( a2 , MonElt->cle) ){
      ajouter_cle( MonArbre , MonElt->cle ) ;
    }
    if ( MonElt->fgauche != NULL ){
      int EnfileG = enfiler( MaFile , MonElt->fgauche );
    }
    if ( MonElt->fdroite != NULL ){
      int EnfileD = enfiler( MaFile , MonElt->fdroite );
    }
  }

  detruire_file( MaFile ) ;
  return MonArbre ; 
}

Arbre_t union_deux_arbres (Arbre_t a1, Arbre_t a2){
  Arbre_t MonArbre = a1 ;
  pfile_t MaFileA1 = creer_file() ;
  int EnfileGA1 = enfiler( MaFileA1 , a1 );

  while ( !file_vide( MaFileA1 ) ){
    pnoeud_t MonElt = defiler( MaFileA1 );
    //printf("Cle : %d \n", MonElt->cle );
    if ( !( ValDansArbre( a1 , MonElt->cle ) ) ){
      ajouter_cle( MonArbre , MonElt->cle ) ;
    }
    if ( MonElt->fgauche != NULL ){
      int EnfileG = enfiler( MaFileA1 , MonElt->fgauche );
    }
    if ( MonElt->fdroite != NULL ){
      int EnfileD = enfiler( MaFileA1 , MonElt->fdroite );
    }
  }

  detruire_file( MaFileA1 ) ;
  return MonArbre ; 
}



// pour la somme des hauters, puis-je faire ca : 
int somme_hauter( Arbre_t a ){
  if ( a == NULL )
  {
    return 0 ;
  } else {
    return 1 + somme_hauter( a->fgauche ) + somme_hauter( a->fdroite ) ; 
  }
  
}

int nb_noeuds( Arbre_t a ){
  if ( a == NULL ){
    return 0 ;
  } else {
    return 1 + nb_noeuds( a->fgauche ) + nb_noeuds( a->fdroite ) ;
  }
}