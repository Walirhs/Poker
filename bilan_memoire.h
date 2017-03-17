/********************************************************************************************************************************************************
*                                                                     Poker                                                                             *
*********************************************************************************************************************************************************
* Fichier: bilan_memoire.h
* Version: 1.00
* Description: Déclaration des fonctions, constantes et macro pour faire un bilan mémoire, ce fichier est adaptable selon le projet en couple avec le .c
*
* Dernière modification de: Kévin
* Faite le: 21/07/2015
*
* Modification faite: Ecriture de ce fichier
*
* Modification à faire: RAS
*
*********************************************************************************************************************************************************/

#ifndef BILAN_MEMOIRE_H
#define BILAN_MEMOIRE_H

    #include "bilan_memoire.c"

/** Déclaration des constantes, macros **/
    #define malloc(X) mon_malloc(X) // La fonction mon_malloc(X) prend comme nom malloc(X)
    #define free(X) mon_free(X)
    #define calloc(Y,X) mon_calloc(Y,X)
    #define fopen(X,Y) mon_fopen(X,Y)
    #define fclose(X) mon_fclose(X)

/** Liens des prototypes de fonctions **/
/* Allocation/Comptabilité Memoire ----------------------------------------------------------------------------------------------------------------------*/
    extern void *mon_malloc (size_t size);
    extern void mon_free (void *ptr);
    extern void *mon_calloc(size_t taille, size_t size);
    extern void *mon_fopen(const char* chemin_fichier,const char* type_operation);
    extern void mon_fclose(FILE* fichier_ouvert);
    extern void bilan_memoire(void);

#endif // BILAN_MEMOIRE_H
