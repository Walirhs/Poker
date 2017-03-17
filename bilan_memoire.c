/********************************************************************************************************************************************************
*                                                                     Poker                                                                             *
*********************************************************************************************************************************************************
* Fichier: bilan_memoire.c
* Version: 1.00
* Description: Fonctions pour la comptabilisation du bilan m�moire
*
* Derni�re modification de: K�vin
* Faite le: 05/10/2015
*
* Modification faite: Ecriture de ce fichier
*
* Modification � faire: RAS
*
*********************************************************************************************************************************************************/

/** D�clarations des variables globales (� �viter en C) **/
// Ici on alloue de la m�moire pour ses variables et on les initialises
// Elles sont alors accessible dans l'ensemble du projet (variable globales)
// alors qu'en extern il ne fait que le  lien a ici pour les autres fonctions (optionnel pour prototype)
int NB_MALLOC=0;
int NB_FREE=0;
int NB_FOPEN=0;
int NB_FCLOSE=0;

/** D�claration des fonctions **/
/* Allocation/Comptabilit� Memoire ---------------------------------------------------------------------------------------------------------------------*/
void *mon_malloc (size_t size){  // Le type size_t permet de r�cup�rer la taille du type mis en param�tre
    NB_MALLOC++;
    return malloc(size); // Retourne simplement un malloc mais incr�mente la variable au passage
}

void mon_free (void *ptr){ // Toujours le m�me principe
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

// Affiche un message en cas de probl�me memoire
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
