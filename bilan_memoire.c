/********************************************************************************************************************************************************
*                                                                     Poker                                                                             *
*********************************************************************************************************************************************************
* Fichier: bilan_memoire.c
* Version: 1.00
* Description: Fonctions pour la comptabilisation du bilan mémoire
*
* Dernière modification de: Kévin
* Faite le: 05/10/2015
*
* Modification faite: Ecriture de ce fichier
*
* Modification à faire: RAS
*
*********************************************************************************************************************************************************/

/** Déclarations des variables globales (à éviter en C) **/
// Ici on alloue de la mémoire pour ses variables et on les initialises
// Elles sont alors accessible dans l'ensemble du projet (variable globales)
// alors qu'en extern il ne fait que le  lien a ici pour les autres fonctions (optionnel pour prototype)
int NB_MALLOC=0;
int NB_FREE=0;
int NB_FOPEN=0;
int NB_FCLOSE=0;

/** Déclaration des fonctions **/
/* Allocation/Comptabilité Memoire ---------------------------------------------------------------------------------------------------------------------*/
void *mon_malloc (size_t size){  // Le type size_t permet de récupérer la taille du type mis en paramètre
    NB_MALLOC++;
    return malloc(size); // Retourne simplement un malloc mais incrémente la variable au passage
}

void mon_free (void *ptr){ // Toujours le même principe
    NB_FREE++;
    free(ptr);
}

void *mon_calloc(size_t taille,size_t size){
    NB_MALLOC++;
    return calloc(taille,size);
}

void *mon_fopen(const char* chemin_fichier, const char* type_operation){
    NB_FOPEN++;
    return fopen(chemin_fichier,type_operation);
}

void mon_fclose(FILE* fichier_ouvert){
    NB_FCLOSE++;
    fclose(fichier_ouvert);
}

// Affiche un message en cas de problème memoire
void bilan_memoire(void){
    FILE* fichier_memoire = NULL;
    fichier_memoire = fopen ("bilan_memoire.txt","w");

    if( NB_MALLOC!=NB_FREE )
        fprintf(fichier_memoire,"Erreur memoire: %d allocations et %d liberation\n",NB_MALLOC,NB_FREE);
    else
        fprintf(fichier_memoire,"Pas d'erreur memoire : %d allocations  et %d liberation !\n",NB_MALLOC,NB_FREE);
    if (NB_FOPEN!=NB_FCLOSE)
        fprintf(fichier_memoire,"Erreur memoire: %d fichiers ouverts et %d fichiers fermes\n",NB_FOPEN,NB_FCLOSE);
    else
        fprintf(fichier_memoire,"Pas d'erreur memoire : %d fichiers ouverts et %d fichiers fermes !\n",NB_FOPEN,NB_FCLOSE);

    fclose(fichier_memoire);
}
