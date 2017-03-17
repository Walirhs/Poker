/********************************************************************************************************************************************************
*                                                                     Poker                                                                             *
*********************************************************************************************************************************************************
* Type: Jeu de carte
* Langage: C
* Annee: 2015
* FAC: Nanterre PARIS X
*
**********************************************************************************************************************************************************
* Fichier: main.c
* Version: 3.50
* Description: Fonction principale
*
* Derniere modification de: Kevin
* Faite le: 24/12/2015
*
* Modification faite: Finalisation du projet
*
*
* Modification a faire:
*
*
*
* Modification a faire (Kevin):
*
*
* Modification a faire (Mateus):
*
*
* Modification a faire (Farid):
*
*
*********************************************************************************************************************************************************/

/** Declaration des bibliotheques ***********************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio.h"
//#include "bilan_memoire.h"

/** Declaration des constantes  *************************************************************************************************************************/
#define NB_CARTE 52
#define NB_J_MAX 10
#define NB_J_MIN 2
#define RELANCE_MAX 4
#define PRE_FLOP 2
#define FLOP 3
#define TURN 4
#define RIVER 5
#define ARGENT_M 100
#define OUI 1
#define NON 0
#define OUI_NON 2
#define NB_TEST_M 50
#define AFFICHAGE 1

/** Delaration des structures de donnes ***************************************************************************************************************/
typedef enum combi{
    BACKDOOR=1,PAIRE,DOUBLE_PAIRE,BRELAN,SUITE,COULEUR,FULL,CARRE,QUINTE_FLUSH
}Combi;

typedef long long valeur_jeu;

typedef struct Combinaison
{
	int jeu[7];
	valeur_jeu valeur;
} Combinaison;

typedef struct param{
    float passivite;
    float bluff;
} Param_IA;

typedef struct Joueur
{
	int argent;
	int mise;
	int dern_mise;
	int main[2];
	int est_IA;
	Param_IA parm;
	int actif_tour;
	int en_jeu;
	Combinaison gagne;
} Joueur;

typedef struct pot{
	int valeur;
	int blind;
	int dern_mise;
	int nb_relance;
	int dealer;
	int nb_joueur;
	int tapis[5];
} Pot;

/** Delaration des prototypes de fonctions *************************************************************************************************************/
/* Fonction d'initialisation */
void initialisation_jeu_de_carte(int *carte);  // Initialise un paquet de 52 cartes (8 a 59)
Param_IA init_param_ia(int est_ia); // Initialise le type Param_IA
void initialisation_joueur(Joueur *groupe, int nb_j, int nb_h); // Initialise une structure Joueur
void reinit_jeu_groupe(Joueur *grp, int nb_joueur); // Elimine les joueurs sans argent et reinit le jeu des joueurs
void init_pot(Pot *pot, int nb_joueur, int dealer); // Initialiser le type Pot

/* Fonction d'affichage */
void en_tete_poker(void); // Esthetique du poker
int choix_mode_jeu(void); // Menu pour choisir le mode de jeu
int choix_nombre_joueur(int min, int max); // Question pour choisir le nombre de joueur
void menu_principal(int *nb_j, int *nb_humain); // Affiche le menu principal
char conversion_symbole(int carte); // Fonction qui permet d'avoir la valeur numerique char pour afficher le symbole de la carte
void afficher_paquet(int *carte); // Fonction qui affiche les cartes du paquet
void afficher_type_joueur(Joueur *j); // Fonction qui affiche les champs de la structure joueur
void afficher_groupe_joueur(Joueur *grp, int nb_j); // Fonction qui affiche le groupe de joueur
void afficher_pot(Pot *pot); // Affiche le pot
void afficher_carte(int carte); // Affiche une carte
void afficher_tapis(Pot *pot,int phase); // Affiche le tapis selon les phase FLOP, TURN, RIVER
void afficher_pot_dans_jeu(Pot *pot, int phase); // Affiche les valeurs interessante du pot dans le jeu
void afficher_joueur_dans_jeu(Joueur *j); // Affiche les valeurs du joueur interessante dans le jeu
void afficher_groupe_joueur_dans_jeu(Joueur *grp, int nb_j); // Affiche la table de joueur en_jeu
void afficher_choix_joueur(Joueur *j); // Affiche les divers choix que le joueur peut effectuer ainsi que sa main
void afficher_main_joueur(Joueur *j); // Affiche la main du joueur
void afficher_main_fin_tour(Joueur *groupe, int nb_joueur); // Affiche la main de tout les joueurs encore présent à la fin du tour
void afficher_gagnant(Joueur *groupe, int nb_joueur); // Affiche le ou les gagnant
void afficher_perdant(Joueur *groupe, int nb_joueur); // Affiche ceux qui sortent du jeu

/* Fonction de gestion cartes */
int fonction_aleatoire(int min, int max); // Fonction aleatoire [min,max]
void melange_les_cartes(int *paquet); // Melange le paquet de carte en melangeant carte par carte
void distribution_carte(int *paquet, Joueur *groupe, int nb_j); // Distribue les cartes entre les joueurs
int indice_carte_paquet(int *paquet); // Retourne l'indice de la premiere carte du paquet
int brulage_carte(int *paquet); // Brule la premiere carte et retourne l'indice de la suivante
void jeu_croupier(int *paquet, Pot *poker); // Va creer le jeu du croupier

/* Fonction de gestion des mises/tour */
int nb_joueur_restant(Joueur *grp, int nb_joueur); // Fonction qui donne le nb de encore en_jeu
int nb_joueur_actif(Joueur *grp, int nb_joueur); // Renvoit le nombre de joueur actif dans le tour
int joueur_suivant(Joueur *grp, int nb_joueur, int indice); // Fonction qui retrouve l'indice du joueur suivant
int tour_suivant(Joueur *grp, int nb_joueur, int indice); // Fonction qui retourne l'indice du joueur actif suivant
int dealer_suivant(Joueur *grp, Pot *p); // Fonction pour trouver un dealer dans le groupe de joueur
void miser(Joueur *j, Pot *p, int mise); // Gere la fonction de mise pour un joueur en actualisant les valeurs en jeu
int peux_blinder(Joueur *j, int valeur_blind); // Indique si une blind est possible
int blindage(Joueur *grp, Pot *pot); // Automatise les petites et grosse blind
int peux_parler(Joueur *j, Pot *pot); // Indique si l'on peut checker ou non
int peux_suivre(Joueur *j, Pot *pot); // Fonction pour savoir si le joueur peux suivre
int peux_relancer(Joueur *j, Pot *pot, int relance); // Fonction pour savoir si l'on peut relancer ou non
void relancer(Joueur *j, Pot *p, int relance); // Fonction pour relancer durant un tour de mise
char recuperation_choix_joueur(void); // Recupère le choix du joueur
int pot_est_egalise(Joueur *grp, Pot *pot); // Verifie que tous les joueurs ont égalisé leur mise
void tour_joueur(Joueur *grp, Joueur *j, Pot *pot,int indice_joueur, int phase); // Gere les actions du joueur quand il joue
void tour_de_mise(Joueur *grp, Pot *pot, int indice_joueur, int phase); // Fait un tour de jeu entre les phases de jeu

/*Fonction de tri */
void tri_tapis_decroissant(Pot *tab);//tri le tapis de facon decroissante
void tri_main_decroissant(Joueur *joueur,int nb_joueur);//tri la main du joueur de facon decroissante
void fusion_tapis_main_dec(Pot *pot,Joueur *joueur);//fusionne le tapis et la mais et le tri de facon decroissante dans combinaison gagnant du joueur
void tri_decroissant_joueur_jeu(Joueur *joueur,int debut,int nb_joueur,int *case_apres_paire,int nb_carte_a_trie);//trie le jeu du joueur par hauteur a partir des cartes non triees en fonction s'il y a eu un carre, un brelan ou une pair
void tri_decroissant(Joueur *joueur,int premier_joueur,int nb_joueur,int debut,int phase);
void tri_par_carre_brelan_paire(Joueur *joueur,int debut, int nb_joueur,int nb_carte_a_trie);//trie le jeu du joueur(combinaison jeu) par carre puis par brelan puis par pair et enfin par la hauteur de la carte
void decale_double(Joueur *joueur,int indice_joueur,int phase);//decale le doublon a la fin du tableau
void fusion_tapis(Joueur *j,Pot *p,int indice_joueur);//fusion basique du main et du tapis

/*Fonction gestion de la valeur des combinaisons des joueur/gagnant*/
void quelle_est_la_valeur_du_jeu(Joueur *joueur,Pot *pot);
valeur_jeu donne_valeur_du_jeu(int indice_combi,Joueur* joueur,int indice_joueur);

/* Fonction qui determine la valeur du jeu et le meilleur jeu de la table*/
/*9*/valeur_jeu est_une_quinte_flush(Joueur *joueur,int indice,int nb_carte_suite,int phase);
/*8*/valeur_jeu est_un_carre(Joueur *joueur,int indice);
/*7*/valeur_jeu est_un_full(Joueur *joueur,int indice);
/*6*/valeur_jeu est_une_couleur(Joueur *joueur,int indice,int phase,int nb_carte);
/*5*/valeur_jeu est_une_suite(Joueur *joueur,int indice,int nb_carte_suite,int phase);
/*4*/valeur_jeu est_un_brelan(Joueur *joueur,int indice);
/*3*/valeur_jeu est_une_double_paire(Joueur *joueur,int indice);
/*2*/valeur_jeu est_une_paire(Joueur *joueur,int indice);
/*1*/valeur_jeu carte_la_plus_haute(Joueur *joueur,int indice);

/* Fonction de repartition des gain */
int nombre_de_gagnant(Joueur *grp, int nb_joueur); // Fonction qui retourne le nombre de gagnant dans le groupe de joueur
int qui_gagne(Joueur *joueur,int nb_joueur);//retourne le nb de gagnant et met la valeur 1 s'il a gagne 0 sion dans la valeur du jeu
int probleme_jeu_argent(Joueur *grp, int pot_val, int nb_j); // Retourne s'il y a un probleme avec l'argent en jeu
void repartition_gain(Joueur *grp, int pot_a_partager, int nb_j,int nb_gagnant); // Fonction qui va repartir les gain entre les joueurs

/* Fonctions pour l'IA */
int main_appariee(int main[2]); // Retourne si la main du joueur a la meme couleur
int main_paire(int main[2]); // Retourne si la main possede une paire
int main_suite(int main[2]); // Retourne si la main a une micro-suite
float pre_flop_pourcentage(Joueur *j); // Fonction qui recupere une probabilité d'ameliore son jeu au pre-flop (valeur forcee)
int full(Joueur *j,int indice_joueur,int phase_jeu); //calcule les outs si le joueur a un full
int couleur(Joueur *j,int indice_joueur,int phase_jeu); //calcule les outs pour un joueur qui une possibilité d avoir une couleur
int suite_2_bouts(Joueur *j,int indice_joueur,int phase_jeu); //calcule les outs d'un suite à deux entrée cad une possibilité d'avoir une suite soit en partant de la fin ou du début
int brelan_outs(Joueur *j,int indice_joueur,int phase_jeu); //calcule les out si le joueur a une brelan
int double_paire_outs(Joueur *j,int indice_joueur,int phase_jeu); //calcule les outs si le joueur a une double paire
int paire_outs(Joueur *j,int indice_joueur,int phase_jeu); //calcule les outs s'il a une paire
int calcule_nb_outs(Joueur *j,int indice_joueur,int phase_jeu);  //calcul les outs en fonction du jeu du joueur à une phase precise
float pourcentage_perte_pot(float mise_a_faire, float argent_en_jeu, float valeur_pot); // Donne le pourcentage d'argent a investir dans le pot
int faut_il_suivre(float pourcent_gain, float pourcent_bon_jeu); // Si le % d'argent à investir au pourcentage d'améliorer son jeu alors on ne mise pas (A LT)
float pourcentage_outs(int nb_outs, int phase); // Calcul le pourcentage d'améliorer son jeu en fonction du nombre de out et la phase de jeu
Combi jeu_joueur_selon_phase(Joueur *grp,int indice_joueur,int phase); // Recupere la combinaison du joueur selon la phase de jeu
float pourcent_selon_combi(Combi JEU); // Donne un pourcentage de mise totale selon le jeu déjà en main du joueur
float pourcentage_de_base_jeu(Joueur *grp,int indice,int phase); // Rend un pourcentage selon jeu du joueur
float fonction_aleatoire_float(float min, float max); // Petit fonction aléatoire pour les chiffres décimaux [min,max[
int est_dans_intervalle(float val, float born_inf, float born_sup); // Fonction qui regarde si val € [born_inf;born_sup]
char choix_de_IA(Joueur *grp, Joueur *IA, Pot *pot, int indice, int phase); // Retourne le choix de l'IA au tour de mise
void nouve_parm_ia(Joueur *groupe,int indice,float nouv_bluff, float nouv_passivite); // Change les parametre de l'IA à l'indice

/* Fonctions fichiers pour tests IA */
void imprimer_fiche_ia(FILE *fic, Joueur *ia);// Imprime la fiche du joueur a la suite du fichier

/** Fonction principale *********************************************************************************************************************************/
int main(void){
      // Met l'aleatoire en place
    srand(time(0));
    // Declaration de variables du main
    int tour_indice=1/*, un_gagnant=NON*/;
	int nb_jeu = 0, nb_manche = 0, nb_j_total = 0;
    int nb_h = 0, i = 0, nombre_gagnant;
	int    *paquet        = (int*)malloc(sizeof(int)*NB_CARTE);
	Pot    *pot_jeu       = (Pot*)malloc(sizeof(Pot));
	Joueur *groupe_joueur = NULL; // Il nous faut le nb de joueur avant d'init
    FILE *archive_test = fopen("Resultat_Test.txt","w");

    int nb_test = NB_TEST_M;
    int stat_joueur[10]={0,0,0,0,0,0,0,0,0,0};

    menu_principal(&nb_j_total,&nb_h);

	groupe_joueur = (Joueur*)malloc(sizeof(Joueur)*nb_j_total);

    while(nb_test--){
    initialisation_joueur(groupe_joueur,nb_j_total,nb_h);

    init_pot (pot_jeu,nb_j_total,fonction_aleatoire(0,nb_j_total-1));

        while(nb_joueur_restant(groupe_joueur,nb_j_total)!=1){
            nb_jeu++, nb_manche++;
            init_pot(pot_jeu,nb_j_total,dealer_suivant(groupe_joueur,pot_jeu));
            // Evite que les parties deviennent trop longue
            if(nb_jeu==10) pot_jeu->blind *= 2;
            if(nb_jeu==30) pot_jeu->blind *= 2;

            initialisation_jeu_de_carte(paquet);
            melange_les_cartes(paquet);

            // Pour le test du gagnant
            distribution_carte(paquet,groupe_joueur,nb_j_total);
            jeu_croupier(paquet,pot_jeu);
            for(i=0;i<nb_j_total;i++)
                fusion_tapis(groupe_joueur,pot_jeu,i);

            if(AFFICHAGE){
                afficher_pot_dans_jeu(pot_jeu,0);
                afficher_groupe_joueur_dans_jeu(groupe_joueur,pot_jeu->nb_joueur);
                system("pause");
            }
            tour_indice = blindage(groupe_joueur,pot_jeu); // retourne l'indice du joueur suivant apres la GB


            for(i=PRE_FLOP;i<=RIVER;i++){
                if(AFFICHAGE){
                    afficher_pot_dans_jeu(pot_jeu,i);
                    afficher_groupe_joueur_dans_jeu(groupe_joueur,pot_jeu->nb_joueur);
                }
                tour_de_mise(groupe_joueur,pot_jeu,tour_indice,i);
                pot_jeu->nb_relance = 0;
            }

            fusion_tapis_main_dec(pot_jeu,groupe_joueur);
            tri_par_carre_brelan_paire(groupe_joueur,0,nb_j_total,6);
            quelle_est_la_valeur_du_jeu(groupe_joueur,pot_jeu);

            nombre_gagnant=qui_gagne(groupe_joueur,pot_jeu->nb_joueur);

            repartition_gain(groupe_joueur, pot_jeu->valeur, pot_jeu->nb_joueur,nombre_gagnant);

            if(AFFICHAGE){
                printf("\n\n__________________________________________________________________________\n");
                afficher_main_fin_tour(groupe_joueur,nb_j_total);
                afficher_gagnant(groupe_joueur,nb_j_total);
                afficher_perdant(groupe_joueur,nb_j_total);
                printf("\n");
                system("pause");
            }

            reinit_jeu_groupe(groupe_joueur,nb_j_total);

        }


        for(i=0;i<nb_j_total;i++){
            if(groupe_joueur[i].en_jeu) stat_joueur[i]++;
        }

    }


    fprintf(archive_test,"___________________Sur %6d test__________________\n",NB_TEST_M);
    for(i=0;i<nb_j_total;i++)
        fprintf(archive_test,"Le joueur %2d\n",i+1),
        imprimer_fiche_ia(archive_test,groupe_joueur+i);
    for(i=0;i<nb_j_total;i++)
        fprintf(archive_test,"Le joueur %2d a gagne %3d fois \n",i,stat_joueur[i]);
    fprintf(archive_test,"____________________Et %6d manches_______________\n",nb_manche);
	system("pause");


    fclose(archive_test);
    free(paquet);
    free(groupe_joueur);
	free(pot_jeu);
    //bilan_memoire();
    return EXIT_SUCCESS;
}

/** Fonction d'initialisation ***************************************************************************************************************************/
// Initialise un paquet de 52 cartes (8 a 59)
void initialisation_jeu_de_carte(int *carte)
{
	int i; // Division par 4 donne la hauteur (1 a 14)
	for(i=0;i<NB_CARTE;i++){ // Modulo par 4 donne la couleur 0=Coeur, 1=Carreau, 2=Trefle, 3=Pique
		*(carte+i)=8+i;
	}
}

// Initialise le type Param_IA
Param_IA init_param_ia(int est_ia){
    Param_IA p;
    p.passivite = (est_ia)? fonction_aleatoire_float(0.2,0.8) : 0 ;
    p.bluff       = (est_ia)? fonction_aleatoire_float(0.2,0.8) : 0 ;
    return p;
}

// Initialise une structure Joueur
void initialisation_joueur(Joueur* groupe, int nb_j, int nb_h){
	int i,j;
	for(i=0;i<nb_j;i++){
        groupe[i].argent    	   = ARGENT_M;
        groupe[i].mise             = 0;
        groupe[i].dern_mise 	   = 0;
		groupe[i].main[0]   	   = groupe[i].main[1] = 0;
        groupe[i].est_IA           = (nb_h-- > 0)? NON : OUI;
        groupe[i].parm             = init_param_ia(groupe[i].est_IA);
		groupe[i].en_jeu   		   = OUI;
		groupe[i].actif_tour       = OUI;
		groupe[i].gagne.valeur     = 0;
		for(j=0;j<7;j++)
			groupe[i].gagne.jeu[j] = 0;
	}
}

// Elimine les joueurs sans argent et reinit le jeu des joueurs
void reinit_jeu_groupe(Joueur *grp, int nb_joueur){
    int indice=0,i,j, nb_joueur_actuel = nb_joueur_restant(grp,nb_joueur) ;
    if(grp[indice].en_jeu==NON) indice= joueur_suivant(grp,nb_joueur,indice);

    for(i=0; i<nb_joueur_actuel ;i++){
        if(!grp[indice].argent) grp[indice].en_jeu = NON;
        grp[indice].mise           = 0;
        grp[indice].dern_mise      = 0;
        grp[indice].main[0]        = 0;
        grp[indice].main[1]        = 0;
        grp[indice].actif_tour     = OUI;
        grp[indice].gagne.valeur   = 0;
        for(j=0;j<7;j++)
            grp[indice].gagne.jeu[j]    = 0;
        indice = joueur_suivant(grp,nb_joueur,indice);
    }
}

// Initialiser le type Pot
void init_pot(Pot *pot,int nb_joueur,int dealer){
	int i;
	pot->valeur  	= 0;
	pot->blind		= ARGENT_M/100;
	pot->dern_mise	= 0;
	pot->nb_relance	= 0;
	pot->dealer		= dealer;
	pot->nb_joueur  = nb_joueur;
	for(i=0;i<5;i++)
		pot->tapis[i]    = 0;
}

/** Fonction d'affichage ***************************************************************************************************************************/
// Esthétique du poker
void en_tete_poker(void){
	system("cls");
	printf("__________________________________________________________________________\n");
	printf("		           Poker: Texas Hold'em                       \n");
	printf("__________________________________________________________________________\n");
}

// Menu pour choisir le mode de jeu
int choix_mode_jeu(void){
    int choix;
    do{
        printf("\nChoisissez le mode de jeu\n");
        printf("[1]: Humain contre Humain\n");
        printf("[2]: Humain contre IA\n");
        printf("[3]: IA     contre IA\n");
        scanf("%d",&choix);
        printf("\n");
    } while(choix<1 || choix>3);
    return choix;
}

// Question pour choisir le nombre de joueur
int choix_nombre_joueur(int min, int max){
	int nb;
	do{
		printf("Entrez le nombre de joueur (%d a %d):\n",min,max);
		scanf("%d",&nb);
		printf("\n");
	} while(nb<min || nb>max);
	return nb;
}

// Affiche le menu principal
void menu_principal(int *nb_j, int *nb_humain){
    en_tete_poker();

	int mode_jeu   = choix_mode_jeu();
    *nb_j = choix_nombre_joueur(NB_J_MIN,NB_J_MAX);

	if(mode_jeu==2){
        printf("Choisissez le nombre d'humain qui jouent\n");
        *nb_humain = choix_nombre_joueur(1,(*nb_j)-1);
	}
	else if (mode_jeu==1) *nb_humain = *nb_j;

	system("cls");
}

// Fonction qui permet d'avoir la valeur numerique char pour afficher le symbole de la carte
char conversion_symbole(int carte)
{
	return 3+ carte%4; //on fait +3 car le coeur commence au 3eme entier des caractere
}

// Fonction qui affiche les cartes du paquet
void afficher_paquet(int *carte)
{
	int i;
	for(i=0;i<NB_CARTE;i++)
        printf( (i%4!=3)? "%2d%c " : "%2d%c\n" ,carte[i]/4 ,conversion_symbole(carte[i]) );
}

// Fonction qui affiche les champs de la structure joueur
void afficher_type_joueur(Joueur *j){
	int k;
	printf("Cagnotte:     %3d\n",     j->argent);
	printf("Mise:         %3d\n",     j->mise);
	printf("Dern_mise:    %3d\n",     j->dern_mise);
	printf("Carte1:       %3d %c\n", (j->main[0])/4, conversion_symbole(j->main[0]) );
	printf("Carte2:       %3d %c\n", (j->main[1])/4, conversion_symbole(j->main[1]) );
	printf("Est_IA:       %3d\n",     j->est_IA);
	printf("Passivite:  %.3f\n",      j->parm.passivite);
	printf("Bluff:        %.3f\n",    j->parm.bluff);
	printf("En_jeu:       %3d\n",     j->en_jeu);
	printf("Actif_tour:   %3d\n",     j->actif_tour);
	printf("Valeur:         %I64u\n", j->gagne.valeur);
	printf("Combinaison: ");
	for(k=0;k<7;k++) afficher_carte(j->gagne.jeu[k]);
    printf("\n\n");
}

// Fonction qui affiche le groupe de joueur
void afficher_groupe_joueur(Joueur *grp, int nb_j){
	int i;
	for(i=0;i<nb_j;i++){
        printf("_________________________________Joueur %2d_________________________________\n",i);
        afficher_type_joueur(grp+i);
	}
}

// Affiche le pot
void afficher_pot(Pot *pot){
 	int i;
	printf("pot->valeur     = %d\n",pot->valeur);
	printf("pot->blind      = %d\n",pot->blind);
	printf("pot->dern_mise  = %d\n",pot->dern_mise);
	printf("pot->nb_relance = %d\n",pot->nb_relance);
	printf("pot->dealer     = %d\n",pot->dealer);
	printf("pot->nb_joueur  = %d\n",pot->nb_joueur);
	printf("pot->tapis      =");
	for(i=0;i<5;i++){
        afficher_carte(pot->tapis[i]);
	}
    printf("\n\n");
}

// Affiche une carte
void afficher_carte(int carte){
    printf(" | %2d %c | ",carte/4,conversion_symbole(carte));
}

// Affiche le tapis selon les phase FLOP, TURN, RIVER
void afficher_tapis(Pot *poker, int phase){
	int i;
	printf("--------------------------------------------------------------------------\n           ");
    for(i=0;i<phase && phase!=PRE_FLOP;i++)
        afficher_carte(poker->tapis[i]);
	printf("\n--------------------------------------------------------------------------\n");
}

// Affiche les valeurs interessante du pot dans le jeu
void afficher_pot_dans_jeu(Pot *pot, int phase){
    en_tete_poker();
    afficher_tapis(pot,phase);
    printf("Dealer:            %4d\n"     ,pot->dealer+1);
    printf("Pot en jeu:        %4d\n"     ,pot->valeur);
    printf("Petite blind:      %4d\n"     ,pot->blind);
    printf("Derniere mise:     %4d\n"     ,pot->dern_mise);
    printf("Nombre de relance: %4d\n"     ,pot->dern_mise);
    printf("\n");
}

// Affiche les valeurs du joueur interessante dans le jeu
void afficher_joueur_dans_jeu(Joueur *j){
    printf("Argent: %3d | Derniere mise: %3d | Actif tour: ",j->argent,j->dern_mise);
    printf( (j->actif_tour)? "OUI\n":"NON\n");
}

// Affiche la table de joueur en_jeu
void afficher_groupe_joueur_dans_jeu(Joueur *grp, int nb_j){
    int indice_joueur = (grp[0].en_jeu)? 0 : joueur_suivant(grp,nb_j,0); //si le joueur 0 est en_jeu sinon le suivant en_jeu
    int i;
    for(i=0;i<nb_joueur_restant(grp,nb_j);i++){
        printf("Joueur %2d: ",indice_joueur+1);
        afficher_joueur_dans_jeu(grp+indice_joueur);
        indice_joueur = joueur_suivant(grp,nb_j,indice_joueur);
    }
    printf("__________________________________________________________________________\n\n");
}

// Affiche les divers choix que le joueur peut effectuer ainsi que sa main
void afficher_choix_joueur(Joueur *j){
    afficher_main_joueur(j);
    printf("\n[p]: Parole | [s]: Suivre | [r]: Relancer | [c]: Se Coucher\n");
}

// Affiche la main du joueur
void afficher_main_joueur(Joueur *j){
    printf("Main: ");
    afficher_carte(j->main[0]);
    afficher_carte(j->main[1]);
}

// Affiche la main de tout les joueurs encore présent à la fin du tour
void afficher_main_fin_tour(Joueur *groupe, int nb_joueur){
    int i;
    for(i=0;i<nb_joueur;i++){
        if(groupe[i].actif_tour==OUI){
            printf("Joueur %2d: ",i+1);
            afficher_main_joueur(groupe+i);
            printf("\n");
        }
    }
}

// Affiche le ou les gagnant
void afficher_gagnant(Joueur *groupe, int nb_joueur){
    int i;

    for(i=0;i<nb_joueur;i++){
        if(groupe[i].gagne.valeur==1)
            printf("Le joueur %2d gagne cette manche!\n",i+1);
    }
}

// Affiche ceux qui sortent du jeu
void afficher_perdant(Joueur *groupe, int nb_joueur){
    int i;

    for(i=0;i<nb_joueur;i++){
        if(groupe[i].en_jeu==NON)
            printf("Le joueur %2d sort du jeu ! Merci d'avoir participe !\n",i+1);
    }
}

/** Fonction de gestion cartes **************************************************************************************************************************/
// Fonction aleatoire [min,max]
int fonction_aleatoire(int min, int max)
{
	return rand()%(max-min+1)+min; // Choisis un chiffre entre min et max compris
}

// Melange le paquet de carte en melangeant carte par carte
void melange_les_cartes(int *paquet)
{
	int c,tmp,i;
	for(i=NB_CARTE-1;i>=0;i--)
	{
        c = fonction_aleatoire(0,i); //Prend un indice aleatoire
		tmp = paquet[i]; //Recupere la valeur de la i-eme carte (partant de la fin)
		paquet[i] = paquet[c]; //Echange la carte avec l'indice c
		paquet[c] = tmp; // avec la i-eme et remonte
	}
}

// Distribue les cartes entre les joueurs
void distribution_carte(int *paquet, Joueur *groupe, int nb_j){
	int i,j,k=0;
	for(j=0;j<2;j++){
		for(i=0;i<nb_j;i++){
			groupe[i].main[j] = paquet[k];
			paquet[k++]= 0;
		}
	}
}

// Retourne l'indice de la premiere carte du paquet
int indice_carte_paquet(int *paquet){
	int i;
	for(i=0;i<NB_CARTE;i++)
		if(paquet[i]!=0)
			return i;
	return 0;
}
// Brule la premiere carte et retourne l'indice de la suivante
int brulage_carte(int *paquet){
	paquet[indice_carte_paquet(paquet)] = 0;
	return indice_carte_paquet(paquet);
}
// Va creer le jeu du croupier
void jeu_croupier(int *paquet, Pot *poker){
    int i, indice = brulage_carte(paquet);

    for(i=0; i<FLOP; i++){
        poker->tapis[i] = paquet[indice];
        paquet[indice++]=0;
    }
    for(i=3;i<RIVER;i++){
        poker->tapis[i] = paquet[indice=brulage_carte(paquet)];
        paquet[indice] = 0;
    }
}

/** Fonction de gestion des mises/tour ******************************************************************************************************************/
// Fonction qui donne le nb de encore en_jeu
int nb_joueur_restant(Joueur *grp, int nb_joueur){
    int i, nb_j_restant=0;
    for(i=0;i<nb_joueur;i++){
       if(grp[i].en_jeu == OUI)
            nb_j_restant++;
    }
    return nb_j_restant;
}

// Renvoit le nombre de joueur actif dans le tour
int nb_joueur_actif(Joueur *grp, int nb_joueur){
    int i, nb_actif=0;
    for(i=0;i<nb_joueur;i++){
        if(grp[i].actif_tour == OUI)
            nb_actif++;
    }
    return nb_actif;
}

// Fonction qui retrouve l'indice du joueur suivant
int joueur_suivant(Joueur *grp,int nb_joueur, int indice){
    do{
        ++indice; //Incrémente l'indice pour regarder le joueur suivant
        indice%=nb_joueur; //Fait le modulo pour passer au joueur 0 si au bout de table
    }
    while(grp[indice].en_jeu==NON); // Boucle tant qu'on trouve pas de joueur en jeu

    return indice;
}

// Fonction qui retourne l'indice du joueur actif suivant
int tour_suivant(Joueur *grp, int nb_joueur, int indice){
    do{
        ++indice;
        indice%=nb_joueur;
    }
    while(grp[indice].actif_tour==NON);

    return indice;
}

// Fonction pour trouver un dealer dans le groupe de joueur
int dealer_suivant(Joueur *grp, Pot *p){
    return p->dealer = joueur_suivant(grp,p->nb_joueur,p->dealer);
}

// Gere la fonction de mise pour un joueur en actualisant les valeurs en jeu
void miser(Joueur *j, Pot *p, int mise){
    j->mise      += mise;
    j->argent    -= mise;
    j->dern_mise =  mise;
    p->valeur    += mise;
    p->dern_mise =  mise;
}

// Indique si une blind est possible
int peux_blinder(Joueur *j, int valeur_blind){
    if(j->argent < valeur_blind) return NON;
    return OUI;
}

// Automatise les petites et grosse blind A_FINIR
int blindage(Joueur *grp, Pot *pot){
    // indice des petites et grosse blind
    int petite_blind = joueur_suivant(grp,pot->nb_joueur,pot->dealer);
    int grosse_blind = joueur_suivant(grp,pot->nb_joueur,petite_blind);

    // dans le cas ou les joueurs ne peuveut assurer la blind
    int big_blind= (peux_blinder(grp+grosse_blind,2*pot->blind))? 2*pot->blind : (grp+grosse_blind)->argent;

    miser(grp+petite_blind, pot, pot->blind);
    miser(grp+grosse_blind, pot, big_blind);

    if( (grp+grosse_blind)->argent == 0 ){
        pot->dern_mise = 2*pot->blind;
    }

	return joueur_suivant(grp,pot->nb_joueur,grosse_blind);
}

// Indique si l'on peut checker ou non
int peux_parler(Joueur *j, Pot *pot){
    if(pot->dern_mise <= j->dern_mise){
		return OUI;
    }
    return NON;
}

// Fonction pour savoir si le joueur peux suivre
int peux_suivre(Joueur *j, Pot *pot){
    // Passe en check automatiquement si pas besoin d'egaliser la mise
    if(peux_parler(j,pot)){
		return -1;
    }
    // Si on peut egaliser la mise
    if(j->argent >= pot->dern_mise){
        return OUI;
    }
    return NON;
}

// Fonction pour savoir si l'on peut relancer ou non
int peux_relancer(Joueur *j, Pot *pot, int relance){
    if(pot->nb_relance == RELANCE_MAX) return NON;

    int mise_a_egaliser = pot->dern_mise - j->dern_mise;
    if(j->argent >= mise_a_egaliser+relance) return OUI;

    return NON;
}

// Fonction pour relancer durant un tour de mise
void relancer(Joueur *j, Pot *p, int relance){
    int dern_mise = j->dern_mise; // garde la valeur des jetons deja mis en jeu par le joueur

    miser(j, p, (p->dern_mise-j->dern_mise)+relance); // egaliser la mise + la relance
    p->nb_relance++;
    p->dern_mise += dern_mise;
}

// Recupère le choix du joueur
char recuperation_choix_joueur(void){
    char choix = 'z';

    choix=getch();

    return choix;
}

// Verifie que tous les joueurs ont egalise leur mise
int pot_est_egalise(Joueur *grp, Pot *pot){
    int i, indice = joueur_suivant(grp,pot->nb_joueur,0);
    int nb_actif = nb_joueur_actif(grp,pot->nb_joueur);
    for(i=0 ; i<nb_actif ; i++){
        if(grp[indice].dern_mise!=pot->dern_mise) return NON;
        indice = joueur_suivant(grp,pot->nb_joueur,indice);
    }
    return OUI;
}

// Gere les actions du joueur quand il joue A_FINIR
void tour_joueur(Joueur *grp, Joueur *j, Pot *pot,int indice_joueur, int phase){
    char choix_joueur = 'z',  dern_mise = 0, dern_mise_pot=0, tour_en_cours = OUI;
    int relance = 2*pot->blind; // petite mise si on est en phase pre-flop/flop
    relance*=( (phase<=FLOP)? 1 : 2); // passe en grosse mise si phase apres flop

	if(AFFICHAGE){
            if(j->est_IA==NON) afficher_choix_joueur(j);
	}

    while(tour_en_cours){
        if(j->est_IA==NON)  choix_joueur = recuperation_choix_joueur();
        else 				choix_joueur = choix_de_IA(grp,j,pot,indice_joueur,phase);

        switch(choix_joueur){
            case 'p': // Parole (check)
                if( peux_parler(j,pot) ){
                    if(AFFICHAGE) printf("Check\n");
                    tour_en_cours = NON;
                }
                break;
            case 's': // Suivre
                dern_mise = j->dern_mise; // recuperer la valeur de la derniere mise du joueur
                switch(peux_suivre(j,pot)){
                    case OUI:
                        miser(j,pot,pot->dern_mise - j->dern_mise); // on egalise la mise du joueur
                        pot->dern_mise = j->dern_mise = j->dern_mise + dern_mise; // on remet la vraie mise dans le nos variables
                        if(AFFICHAGE) printf("Suis pour %d\n",pot->dern_mise-dern_mise);
                        break;
                    case NON:
                        miser(j,pot,j->argent);
                        j->actif_tour = NON;
                        if(AFFICHAGE) printf("Tapis de %d\n",j->dern_mise);
                        break;
                    case -1:
                        if(AFFICHAGE) printf("Parole\n");
                        break;
                }
                tour_en_cours = NON;
                break;
            case 'r': // Relancer
                dern_mise_pot = pot->dern_mise;
                dern_mise = j->dern_mise; // garde la valeur des jetons deja mis en jeu par le joueur
                if(peux_relancer(j,pot,relance)){
                    relancer(j,pot,relance);
                    if(AFFICHAGE)printf("Suis de %d\n",dern_mise_pot-dern_mise);
                    if(AFFICHAGE)printf("Et relance de %d\n",relance);
                    tour_en_cours = NON;
                }
                else{
                    if(j->est_IA==OUI){
                        choix_joueur = 's';
                        tour_en_cours = NON;
                    }
                }
                break;
            case 'c': // Se coucher
                if(AFFICHAGE)printf("Se couche\n");
                j->actif_tour= NON;
                tour_en_cours = NON;
                break;
            default:
                break;
        }
    }

}

// Fait un tour de jeu entre les phases de jeu A_FINIR
void tour_de_mise(Joueur *grp, Pot *pot, int indice_joueur, int phase){
    int i, nb_restant = nb_joueur_actif(grp,pot->nb_joueur);


    if(grp[indice_joueur].actif_tour==NON) indice_joueur = tour_suivant(grp,pot->nb_joueur,indice_joueur);

    for(i=0; i < nb_restant+(nb_restant*pot->nb_relance)&& nb_restant>1 ; i++, nb_restant=nb_joueur_actif(grp,pot->nb_joueur) ){
        if(AFFICHAGE) printf("_________________________________Joueur %2d________________________________\n",indice_joueur+1);
        tour_joueur(grp,grp+indice_joueur,pot,indice_joueur,phase);
		if(AFFICHAGE)  system("pause"), printf("\n");
        indice_joueur = tour_suivant(grp,pot->nb_joueur,indice_joueur);
        // Les joueurs ne peuvent plus relancer et on déjà égalise la mise du pot donc on sort de la boucle
        if(pot_est_egalise(grp,pot) && pot->nb_relance==RELANCE_MAX) break;
    }

    // Il faut mettre les dernieres mises des joueurs a 0 a la fin du tour de mise pour le prochain
    for(i=0; i<pot->nb_joueur; i++){
        grp[i].dern_mise = 0;
    }
    pot->dern_mise = 0;
}

/** Fonction permettant de savoir le vainqueur ******************************************************************************************************************/
//tri le tapis de facon decroissante
void tri_tapis_decroissant(Pot *tab)
{
	int i,tmp,taille=5;

	while(taille){
		for(i=0;i<taille-1;i++){
			if(tab->tapis[i]<tab->tapis[i+1]){
				tmp=tab->tapis[i+1];
				tab->tapis[i+1]=tab->tapis[i];
				tab->tapis[i]=tmp;
			}
		}
		taille--;
	}
}

//tri la main du joueur de facon decroissante
void tri_main_decroissant(Joueur *joueur,int nb_joueur)
{
	int i,tmp;
	for(i=0;i<nb_joueur;i++){
		if(joueur[i].main[0]<joueur[i].main[1]){
			tmp=joueur[i].main[0];
			joueur[i].main[0]=joueur[i].main[1];
			joueur[i].main[1]=tmp;
		}
	}
}

//fusionne le tapis et la main et le trie de facon decroissante dans jeu dans combinaison gagnant du joueur
void fusion_tapis_main_dec(Pot *pot,Joueur *joueur)
{
	tri_main_decroissant(joueur,pot->nb_joueur);
	tri_tapis_decroissant(pot);
	int i=0,j=0,k=0,l=0;

	for(i=0;i<pot->nb_joueur;i++){
		j=0,k=0,l=0;

		while(l<7){
			if(j<2 && joueur[i].main[j]>=pot->tapis[k]){
				joueur[i].gagne.jeu[l]=joueur[i].main[j], j++;
			}
			else{
				if(k<5){
					joueur[i].gagne.jeu[l]=pot->tapis[k];
					k++;
				}
                else joueur[i].gagne.jeu[l]=joueur[i].main[j], j++;
			}
			l++;
		}
	}
}

//trie le jeu du joueur par hauteur a partir des cartes non triees en fonction s'il y a eu un carre, un brelan ou une pair
void tri_decroissant_joueur_jeu(Joueur *joueur,int debut,int nb_joueur,int* case_apres_paire,int nb_carte_a_trie)
{
	int i,j,k,tmp,taille;
	for(j=debut;j<nb_joueur;j++)
	{
		taille=nb_carte_a_trie;
		for (i=0;i<=taille-1;i++){
  			for (k=*(case_apres_paire+j);k<=taille-1-i;k++){
  				if (joueur[j].gagne.jeu[k]<joueur[j].gagne.jeu[k+1]){
  					tmp=joueur[j].gagne.jeu[k+1];
					joueur[j].gagne.jeu[k+1]=joueur[j].gagne.jeu[k];
					joueur[j].gagne.jeu[k]=tmp;
  				}
  			}
		}
	}
	free(case_apres_paire);
}

//trie le jeu du joueur par hauteur a partir de la variable debut
void tri_decroissant(Joueur *joueur,int premier_joueur,int nb_joueur,int debut,int phase)
{
	int i,j,k,tmp,taille;
	for(j=premier_joueur;j<nb_joueur;j++)
	{
		taille=phase;
		for (i=0;i<=taille-1;i++){
  			for (k=debut;k<=taille-1-i;k++){
  				if (joueur[j].gagne.jeu[k]<joueur[j].gagne.jeu[k+1]){
  					tmp=joueur[j].gagne.jeu[k+1];
					joueur[j].gagne.jeu[k+1]=joueur[j].gagne.jeu[k];
					joueur[j].gagne.jeu[k]=tmp;
  				}
  			}
		}
	}
}

//trie le jeu du joueur(combinaison jeu) par carre puis par brelan puis par pair et enfin par la hauteur de la carte
void tri_par_carre_brelan_paire(Joueur *joueur,int debut, int nb_joueur,int nb_carte_a_trie)
{
//    nb_carte_a_trie--;
	int i,j,k,l=0,tmp,tab[13],*derniere_valeur=malloc(sizeof(int)*nb_joueur);
	for(i=0;i<13;i++)
		tab[i]=0;

	for(k=debut;k<nb_joueur;k++){
		l=0;

		for(i=0;i<=nb_carte_a_trie;i++){
			for(j=12;j>=0;j--){
				if(joueur[k].gagne.jeu[i]/4==j+2)
					tab[j]++;
			}
		}
		for(j=12;j>=0;j--)
		{
			if(tab[j]==4){
				for(i=0;i<=nb_carte_a_trie;i++){
					if(joueur[k].gagne.jeu[i]/4==j+2){
						tmp=joueur[k].gagne.jeu[l];
						joueur[k].gagne.jeu[l]=joueur[k].gagne.jeu[i];
						joueur[k].gagne.jeu[i]=tmp;
						l++;
					}
				}
			}
		}
		for(j=12;j>=0;j--){
			if(tab[j]==3){
				for(i=0;i<=nb_carte_a_trie;i++){
					if(joueur[k].gagne.jeu[i]/4==j+2){
						tmp=joueur[k].gagne.jeu[l];
						joueur[k].gagne.jeu[l]=joueur[k].gagne.jeu[i];
						joueur[k].gagne.jeu[i]=tmp;
						l++;
					}
				}

			}
		}
		for(j=12;j>=0;j--)
		{
			if(tab[j]==2){
				for(i=0;i<=nb_carte_a_trie;i++){
					if(joueur[k].gagne.jeu[i]/4==j+2){
						tmp=joueur[k].gagne.jeu[l];
						joueur[k].gagne.jeu[l]=joueur[k].gagne.jeu[i];
						joueur[k].gagne.jeu[i]=tmp;
						l++;
					}
				}

			}
		}
		derniere_valeur[k]=l;//enregistre dans joueur indice du tableau n'a pas encore ete trie pour ensuite faire un tri par hauteur en commencant par cette indice
//		printf("\nJ%d %d\n",k,l);
		for(i=0;i<13;i++)
			tab[i]=0;

	}
	tri_decroissant_joueur_jeu(joueur,debut,nb_joueur,derniere_valeur,nb_carte_a_trie);//trie par hauteur tout ce qu n'a pas ete trie
}

//decale le doublon a la fin du tableau
void decale_double(Joueur *joueur,int indice_joueur,int phase)
{
	int i,j,tmp,a=6*3;

	for(i=0;i<a;i++){
		if((joueur+indice_joueur)->gagne.jeu[i]/4 == (joueur+indice_joueur)->gagne.jeu[i+1]/4)
		{
			for(j=i;j<phase;j++){ //suite sur 7 cartes phase=7 =>j<6
				tmp=(joueur+indice_joueur)->gagne.jeu[j];
				(joueur+indice_joueur)->gagne.jeu[j]=(joueur+indice_joueur)->gagne.jeu[j+1];
				(joueur+indice_joueur)->gagne.jeu[j+1]=tmp;
			}

		}
		if(i+1==6 && a>6)i=0,a=a-6;
	}
}

//fusionne le tapis sans le trier
void fusion_tapis(Joueur *j,Pot *p,int indice_joueur)
{
	int i,k=0;
	(j+indice_joueur)->gagne.jeu[0]=(j+indice_joueur)->main[0];
	(j+indice_joueur)->gagne.jeu[1]=(j+indice_joueur)->main[1];
	for(i=2;i<7;i++)
		(j+indice_joueur)->gagne.jeu[i]=p->tapis[k],k++;
}

/** Fonction gestion de la valeur des combinaisons des joueur/gagnant *******************************************************************************************/
//affecte au joueur les valeurs correpondant a leur jeu
void quelle_est_la_valeur_du_jeu(Joueur *joueur,Pot *pot)
{
	int i,suite=4;
	for(i=0;i<pot->nb_joueur;i++)
	{
		if(est_une_quinte_flush(joueur,i,suite,7) != 0)
			(joueur+i)->gagne.valeur=est_une_quinte_flush(joueur,i,suite,7);
		else if(est_un_carre(joueur,i) != 0)
			(joueur+i)->gagne.valeur=est_un_carre(joueur,i);
		else if(est_un_full(joueur,i) != 0)
			(joueur+i)->gagne.valeur=est_un_full(joueur,i);
		else if(est_une_couleur(joueur,i,6,5) != 0)
			(joueur+i)->gagne.valeur=est_une_couleur(joueur,i,6,5);
		else if(est_une_suite(joueur,i,suite,7) != 0)
			(joueur+i)->gagne.valeur=est_une_suite(joueur,i,suite,7);
		else if(est_un_brelan(joueur,i) != 0)
			(joueur+i)->gagne.valeur=est_un_brelan(joueur,i);
        else if(est_une_double_paire(joueur,i) != 0)
			(joueur+i)->gagne.valeur=est_une_double_paire(joueur,i);
        else if(est_une_paire(joueur,i) != 0)
			(joueur+i)->gagne.valeur=est_une_paire(joueur,i);
		else
			(joueur+i)->gagne.valeur=carte_la_plus_haute(joueur,i);
        // Met 0 à la valeur de gagne pour le joueur qui n'est plus en jeu de base et sort
        if( (joueur+i)->actif_tour == NON) (joueur+i)->gagne.valeur = 0;
	}

}

//determine la valeur du jeu du joueur en fonction de ces 5 meilleurs cartes et l'indice de la combinaison
valeur_jeu donne_valeur_du_jeu(int indice_combi, Joueur* joueur, int indice_joueur)
{
	int i;
	valeur_jeu cmpt= 100000000,valeur= 10000000000; //1+ 5x2 = 11 chiffres pour 1 combi et 5 cartes

	if((joueur+indice_joueur)->en_jeu)
	{
		valeur*=indice_combi;
		for(i=0;i<5;i++){    //Récupere la hauteur des carte (deja triees) du jeu et la place dans valeur
            valeur += ( (joueur+indice_joueur)->gagne.jeu[i]/4 )*cmpt;
            cmpt   *= 0.01; // diminue la puissance de 10^-2 pour placer la carte suivante
		}
	}
	else valeur=0;

	return valeur;
}

/** Fonction qui determine la valeur du jeu et le meilleur jeu de la table***************************************************************************************/
//s il y a une quinte flush retourne la valeur de la suite du joueur sinon retourne 0
valeur_jeu est_une_quinte_flush(Joueur *joueur,int indice_joueur,int nb_carte_suite_couleur,int phase)
{
	int cmpt,tmp,une_quinte_flush=0,a,i,j,derniere_case;

	tri_decroissant(joueur,indice_joueur,indice_joueur+1,0,phase);
	decale_double(joueur,indice_joueur,phase);

	for(j=0;j<=phase-nb_carte_suite_couleur;j++){
		cmpt=0;
		a=4+j;
		for(i=j;i<a && une_quinte_flush == 0;i++){
			if((joueur+indice_joueur)->gagne.jeu[i]/4 == (joueur+indice_joueur)->gagne.jeu[i+1]/4 +1 && (joueur+indice_joueur)->gagne.jeu[i]%4 == (joueur+indice_joueur)->gagne.jeu[i+1]%4){
				cmpt++;
			}
			if(cmpt==nb_carte_suite_couleur) une_quinte_flush++,derniere_case=i+1;//nb_carte_suite_couleur==4 pour trouver une qf a 5 cartes
		}
	}

	if(une_quinte_flush == 0){
		if((joueur+indice_joueur)->gagne.jeu[0]/4 == 14){

			(joueur+indice_joueur)->gagne.jeu[0] = 4+(joueur+indice_joueur)->gagne.jeu[0]%4;

			tri_decroissant(joueur,indice_joueur,indice_joueur+1,0,phase);
			decale_double(joueur,indice_joueur,phase);

			for(j=0;j<=phase-nb_carte_suite_couleur;j++){
				cmpt=0;
				a=4+j;
				for(i=j;i<a && une_quinte_flush == 0;i++){
					if((joueur+indice_joueur)->gagne.jeu[i]/4 == (joueur+indice_joueur)->gagne.jeu[i+1]/4 +1 && (joueur+indice_joueur)->gagne.jeu[i]%4 == (joueur+indice_joueur)->gagne.jeu[i+1]%4){
						cmpt++;
					}
					if(cmpt==nb_carte_suite_couleur) une_quinte_flush++,derniere_case=i+1;//nb_carte_suite_couleur==4 une af a 5 cartes
				}
			}
		}
	}
	if(une_quinte_flush>0){
		int indice_debut=derniere_case-4;
		int nb_echange=5*(indice_debut);

		if(nb_echange==0) nb_echange=-1;
		for(i=0;i<=nb_echange;i++)
		{
			if(i==6 && nb_echange>5){
				i=0;
				nb_echange=nb_echange-5;
			}
			tmp=(joueur+indice_joueur)->gagne.jeu[i];
			(joueur+indice_joueur)->gagne.jeu[i]=(joueur+indice_joueur)->gagne.jeu[i+1];
			(joueur+indice_joueur)->gagne.jeu[i+1]=tmp;
		}
		tri_decroissant(joueur,indice_joueur,indice_joueur+1,nb_carte_suite_couleur+1,phase);
		return donne_valeur_du_jeu(9,joueur,indice_joueur);
	}
	else{
		for(i=0;i<7;i++){
			if((joueur+indice_joueur)->gagne.jeu[i]/4==1){
				(joueur+indice_joueur)->gagne.jeu[i] = 14*4+(joueur+indice_joueur)->gagne.jeu[i]%4;
			}
		}
		tri_par_carre_brelan_paire(joueur,indice_joueur,indice_joueur+1,phase);
	}
	return 0;
}


//s il y a un carre retourne la valeur de la suite du joueur sinon retourne 0
valeur_jeu est_un_carre(Joueur *joueur,int indice_joueur)
{
	if((joueur+indice_joueur)->gagne.jeu[0]/4==(joueur+indice_joueur)->gagne.jeu[1]/4 && (joueur+indice_joueur)->gagne.jeu[1]/4==(joueur+indice_joueur)->gagne.jeu[2]/4 && (joueur+indice_joueur)->gagne.jeu[1]/4==(joueur+indice_joueur)->gagne.jeu[3]/4 ){
		return donne_valeur_du_jeu(8,joueur,indice_joueur);
	}

	return 0;
}

//s il y a un full retourne la valeur de la suite du joueur sinon retourne 0
valeur_jeu est_un_full(Joueur *joueur,int indice_joueur)
{
	int un_full=0;

	if((joueur+indice_joueur)->gagne.jeu[0]/4==(joueur+indice_joueur)->gagne.jeu[1]/4 && (joueur+indice_joueur)->gagne.jeu[1]/4==(joueur+indice_joueur)->gagne.jeu[2]/4){
		un_full++;
	}
	if((joueur+indice_joueur)->gagne.jeu[3]/4==(joueur+indice_joueur)->gagne.jeu[4]/4){
		un_full++;
	}

	if(un_full==2){
		return donne_valeur_du_jeu(7,joueur,indice_joueur);
	}

	return 0;
}
//s il y a une couleur retourne la valeur de la suite du joueur sinon retourne 0
valeur_jeu est_une_couleur(Joueur *joueur,int indice_joueur,int phase,int nb_carte)
{
	int cmpt,tmp,i,j,une_couleur=0,derniere_couleur=0;

	tri_decroissant(joueur,indice_joueur,indice_joueur+1,0,phase);
	for(j=0;j<=3;j++){
		cmpt=0;

		for(i=0;i<=phase && derniere_couleur==0;i++){//phase == 7 pour une couleur sur 7 cartes
			if((joueur+indice_joueur)->gagne.jeu[i]%4 == j)
				cmpt++;
			if(cmpt==nb_carte){ //cmpt==5 pour une couleur sur 7 cartes
				une_couleur++;
				derniere_couleur=j;
			}
		}
	}

	if(une_couleur>0){
		cmpt=0;

		for(i=0;i<phase;i++)
		{
			if((joueur+indice_joueur)->gagne.jeu[i]%4 == derniere_couleur && cmpt<6)
			{
				tmp=(joueur+indice_joueur)->gagne.jeu[cmpt];
				(joueur+indice_joueur)->gagne.jeu[cmpt] =(joueur+indice_joueur)->gagne.jeu[i];
				(joueur+indice_joueur)->gagne.jeu[i]=tmp;
				cmpt++;
			}
		}
		tri_decroissant(joueur,indice_joueur,indice_joueur+1,nb_carte,phase);//si 7 cartes nb_cartes=5
		return donne_valeur_du_jeu(6,joueur,indice_joueur);
	}

	tri_par_carre_brelan_paire(joueur,indice_joueur,indice_joueur+1,phase);
	return 0;
}

//s il y a une suite return la valeur de la suite du joueur sinon retourne 0
valeur_jeu est_une_suite(Joueur *joueur,int indice_joueur,int nb_carte_suite,int phase)
{
	int cmpt,tmp,une_suite=0,i,a,j,derniere_case=0;

	tri_decroissant(joueur,indice_joueur,indice_joueur+1,0,phase);
	decale_double(joueur,indice_joueur,phase);

	for(j=0;j<=phase-nb_carte_suite;j++){//pour suite sur 7 carte RIVER+1=6 nb_carte=4 6-4=2
		cmpt=0;
		a=4+j;
		for(i=j;i<a && une_suite == 0;i++){
			if((joueur+indice_joueur)->gagne.jeu[i]/4 == (joueur+indice_joueur)->gagne.jeu[i+1]/4 +1){
				cmpt++;
			}
			if(cmpt==4) une_suite++,derniere_case=i+1;
		}
	}

	if(une_suite == 0){
		if((joueur+indice_joueur)->gagne.jeu[0]/4 == 14){

			(joueur+indice_joueur)->gagne.jeu[0] = 4+(joueur+indice_joueur)->gagne.jeu[0]%4;

			tri_decroissant(joueur,indice_joueur,indice_joueur+1,0,phase);
			decale_double(joueur,indice_joueur,phase);

			for(j=0;j<=phase-nb_carte_suite;j++){
				cmpt=0;
				a=4+j;
				for(i=j;i<a && une_suite == 0;i++){
					if((joueur+indice_joueur)->gagne.jeu[i]/4 == (joueur+indice_joueur)->gagne.jeu[i+1]/4 +1){
						cmpt++;
					}
					if(cmpt==4) une_suite++,derniere_case=i+1;
				}
			}
		}
	}
	if(une_suite>0){
		int indice_debut=derniere_case-4;
		int nb_echange=5*(indice_debut);

		if(nb_echange==0) nb_echange=-1;
		for(i=0;i<=nb_echange;i++)
		{
			if(i==6 && nb_echange>5){
				i=0;
				nb_echange=nb_echange-5;
			}
			tmp=(joueur+indice_joueur)->gagne.jeu[i];
			(joueur+indice_joueur)->gagne.jeu[i]=(joueur+indice_joueur)->gagne.jeu[i+1];
			(joueur+indice_joueur)->gagne.jeu[i+1]=tmp;
		}
		tri_decroissant(joueur,indice_joueur,indice_joueur+1,nb_carte_suite+1,phase);
		return donne_valeur_du_jeu(5,joueur,indice_joueur);
	}
	else{
		for(i=0;i<7;i++){
			if((joueur+indice_joueur)->gagne.jeu[i]/4==1){
				(joueur+indice_joueur)->gagne.jeu[i] = 14*4+(joueur+indice_joueur)->gagne.jeu[i]%4;
			}
		}
		tri_par_carre_brelan_paire(joueur,indice_joueur,indice_joueur+1,phase);
	}
	return 0;
}

//s il y a brelan return la valeur du brelan du joueur sinon retourne 0
valeur_jeu est_un_brelan(Joueur *joueur,int indice_joueur)
{
	if((joueur+indice_joueur)->gagne.jeu[0]/4==(joueur+indice_joueur)->gagne.jeu[1]/4 && (joueur+indice_joueur)->gagne.jeu[1]/4==(joueur+indice_joueur)->gagne.jeu[2]/4){
		return donne_valeur_du_jeu(4,joueur,indice_joueur);
	}

	return 0;
}

//s il y a une double paire return sa valeur sinon retourne 0
valeur_jeu est_une_double_paire(Joueur *joueur,int indice_joueur)
{
	if((joueur+indice_joueur)->gagne.jeu[0]/4==(joueur+indice_joueur)->gagne.jeu[1]/4 && (joueur+indice_joueur)->gagne.jeu[2]/4==(joueur+indice_joueur)->gagne.jeu[3]/4){
		return donne_valeur_du_jeu(3,joueur,indice_joueur);
	}

	return 0;
}

//s il y a une paire retourne sa valeur sinon return 0
valeur_jeu est_une_paire(Joueur *joueur,int indice_joueur)
{
	if((joueur+indice_joueur)->gagne.jeu[0]/4==(joueur+indice_joueur)->gagne.jeu[1]/4){
		return donne_valeur_du_jeu(2,joueur,indice_joueur);
	}

	return 0;
}

//dertermine la valeur du jeu du joueur par rappor au plus haute carte
valeur_jeu carte_la_plus_haute(Joueur *joueur,int indice_joueur)
{
	return donne_valeur_du_jeu(1,joueur,indice_joueur);
}

/** Fonction de repartition des gains *******************************************************************************************************************/
// Fonction qui retourne le nombre de gagnant dans le groupe de joueur
int nombre_de_gagnant(Joueur *grp, int nb_joueur){
    int i,k=0;
    for(i=0;i<nb_joueur;i++)
        if(grp[i].gagne.valeur) k++;

    return k;
}

//determine le gagnant retourne le nb de gagnant
int qui_gagne(Joueur *joueur,int nb_joueur)
{
    int i;

	valeur_jeu tmp = joueur[0].gagne.valeur;
	for(i=0;i<nb_joueur-1;i++){
		if(tmp<joueur[i+1].gagne.valeur){
			tmp=joueur[i+1].gagne.valeur;
		}
	}

    for(i=0;i<nb_joueur;i++){
		joueur[i].gagne.valeur=(int)(joueur[i].gagne.valeur/tmp);
	}

	return nombre_de_gagnant(joueur,nb_joueur);
}

// Retourne s'il y a un probleme avec l'argent en jeu
int probleme_jeu_argent(Joueur *grp, int pot_val, int nb_j){
    int i,somme=0;

    for(i=0; i<nb_j ;i++) somme+=grp[i].argent;

    somme+=pot_val;

    return (somme != ARGENT_M*nb_j)? OUI : NON;
}

// Fonction qui va repartir les gain entre les joueurs A_FINIR
void repartition_gain(Joueur *grp, int pot_a_partager, int nb_j,int nb_gagnant){
  	int i=0, argent_manquant=0, part_joueur = 0;

  	part_joueur     = pot_a_partager/nb_gagnant;
  	argent_manquant = pot_a_partager - part_joueur*nb_gagnant;

  	for(i=tour_suivant(grp,nb_j,0); nb_gagnant || argent_manquant; i=tour_suivant(grp,nb_j,i) ){
  		if(grp[i].gagne.valeur && nb_gagnant--){
            grp[i].argent += part_joueur;
            if(argent_manquant) grp[i].argent++, argent_manquant--;
  		}
  	}
}

/** Fonction pour l'IA **********************************************************************************************************************************/
// Retourne si la main du joueur a la meme couleur
int main_appariee(int main[2]){
    if(main[0]%4 == main[1]%4) return OUI;
    return NON;
}

// Retourne si la main possede une paire
int main_paire(int main[2]){
    if(main[0]/4 == main[1]/4) return OUI;
    return NON;
}

// Retourne si la main a une micro-suite
int main_suite(int main[2]){
    if(  main[0]/4 == (main[1]/4)+1) return OUI;
    if( (main[0]/4)+1 == main[1]/4 ) return OUI;
    return NON;
}

// Fonction qui recupere une probabilité d'ameliore son jeu au pre-flop (valeur forcee)
float pre_flop_pourcentage(Joueur *j){
    float pourcentage = 0;
    tri_main_decroissant(j,1);
    float valeur_main = (float)(j->main[0]/4 + j->main[1]/4)/100;

    if(main_appariee(j->main)) pourcentage += 0.03;
    else if(valeur_main>0.07)  pourcentage -= 0.03;

    if(main_paire(j->main)) pourcentage += .1;

    if(main_suite(j->main)) pourcentage += 0.02;

    if(valeur_main > 0.07) valeur_main -= 0.04;
    if(valeur_main > 0.10) valeur_main -= 0.03;
    if(valeur_main > 0.13) valeur_main -= 0.05;
    if(valeur_main > 0.16) valeur_main -= 0.06;

    pourcentage += valeur_main ;

    return pourcentage;
}

//calcule les outs si le joueur a un full
int full(Joueur *j,int indice_joueur,int phase_jeu)
{
	phase_jeu++;
	tri_par_carre_brelan_paire(j,indice_joueur,indice_joueur+1,phase_jeu);
	if(est_un_full(j,indice_joueur)) {
		return 1;//1 pour ameliorer brelan en carre + 6 cartes pour faire full
	}
	return 0;
}

//calcule les outs pour un joueur qui une possibilité d avoir une couleur
int couleur(Joueur *j,int indice_joueur,int phase_jeu)
{
	phase_jeu++;
	if(!est_une_couleur(j,indice_joueur,phase_jeu,5))
	{
		if(est_une_couleur(j,indice_joueur,phase_jeu,4)) {
			return 9;//1 pour ameliorer brelan en carre + 6 cartes pour faire full
		}
	}
	return 0;
}

//calcule les outs d'un suite à deux entrée cad une possibilité d'avoir une suite soit en partant de la fin ou du début
int suite_2_bouts(Joueur *j,int indice_joueur,int phase_jeu)
{
	int i;
	phase_jeu++;
	if(est_une_suite(j,indice_joueur,4,phase_jeu)==0){
		if(est_une_suite(j,indice_joueur,3,phase_jeu)){
			for(i=0;i<=phase_jeu;i++){
				if((j+indice_joueur)->gagne.jeu[i]/4==14 || (j+indice_joueur)->gagne.jeu[i]/4==1)
				{
					return 4;
				}
			}
			return 8;//1 pour ameliorer brelan en carre + 6 cartes pour faire full
		}
	}
	return 0;
}

//calcule les out si le joueur a une brelan
int brelan_outs(Joueur *j,int indice_joueur,int phase_jeu)
{
	phase_jeu++;
	tri_par_carre_brelan_paire(j,indice_joueur,indice_joueur+1,phase_jeu);
	if(phase_jeu-1 == TURN) phase_jeu=3;
	else phase_jeu=0;
	if(est_un_brelan(j,indice_joueur)){
		return 7+phase_jeu;//1 pour ameliorer brelan en carre + 6 cartes pour faire full
	}
	return 0;
}

//calcule les outs si le joueur a une double paire
int double_paire_outs(Joueur *j,int indice_joueur,int phase_jeu)
{
	phase_jeu++;
	tri_par_carre_brelan_paire(j,indice_joueur,indice_joueur+1,phase_jeu);
	if(est_une_double_paire(j,indice_joueur)){
		return 4;//1 pour ameliorer brelan en carre + 6 cartes pour faire full
	}
	return 0;
}

//calcule les outs s'il a une paire
int paire_outs(Joueur *j,int indice_joueur,int phase_jeu)
{
	phase_jeu++;
	tri_par_carre_brelan_paire(j,indice_joueur,indice_joueur+1,phase_jeu);
	if(est_une_paire(j,indice_joueur)){
		return 11;//1 pour ameliorer brelan en carre + 6 cartes pour faire full
	}
	return 0;
}

//calcul les outs en fonction du jeu du joueur à une phase precise
int calcule_nb_outs(Joueur *j,int indice_joueur,int phase_jeu){

	int outs=0;
	outs+=couleur(j,indice_joueur,phase_jeu);
	if(full(j,indice_joueur,phase_jeu))
	{
		outs+=full(j,indice_joueur,phase_jeu);
	}
	else{
		if(brelan_outs(j,indice_joueur,phase_jeu)){
			outs+=brelan_outs(j,indice_joueur,phase_jeu);
		}
		else{
			if(double_paire_outs(j,indice_joueur,phase_jeu))
			{
				outs+=double_paire_outs(j,indice_joueur,phase_jeu);
			}
			else{
				outs+=paire_outs(j,indice_joueur,phase_jeu);
			}
		}
	}
	outs+=suite_2_bouts(j,indice_joueur,phase_jeu);
	return outs;
}

// Donne le pourcentage d'argent a investir dans le pot
float pourcentage_perte_pot(float mise_a_faire, float argent_en_jeu, float valeur_pot){
    return argent_en_jeu +=mise_a_faire, argent_en_jeu/(valeur_pot+argent_en_jeu);
}

// Si le % d'argent à investir est supérieur au pourcentage d'améliorer son jeu alors on ne mise pas (A LT)
int faut_il_suivre(float pourcent_gain, float pourcent_bon_jeu){
    return (pourcent_gain<=pourcent_bon_jeu)? OUI : NON ;
}

// Calcul le pourcentage d'améliorer son jeu en fonction du nombre de out et la phase de jeu
float pourcentage_outs(int nb_outs, int phase){
    float nb_carte_privee = NB_CARTE -2 - phase;
        return (float)(nb_outs/nb_carte_privee);
}

// Recupere la combinaison du joueur selon la phase de jeu
Combi jeu_joueur_selon_phase(Joueur *grp, int indice_joueur,int phase){
    phase++; // Flop:3 donc 4 or for(i=0;i<=4.. cf fonction plus bas
    tri_par_carre_brelan_paire(grp,indice_joueur,indice_joueur+1,phase);

    if(est_une_quinte_flush(grp,indice_joueur,4,phase))   return QUINTE_FLUSH;
    if(est_un_carre(grp,indice_joueur))                   return CARRE;
    if(est_un_full(grp,indice_joueur))                    return FULL;
    if(est_une_couleur(grp,indice_joueur,phase,5))        return COULEUR;
    if(est_une_suite(grp,indice_joueur,4,phase))          return SUITE;
    if(est_un_brelan(grp,indice_joueur))                  return BRELAN;
    if(est_une_double_paire(grp,indice_joueur))           return DOUBLE_PAIRE;
    if(est_une_paire(grp,indice_joueur))                  return PAIRE;

                    return BACKDOOR;
}

// Donne un pourcentage de mise totale selon le jeu déjà en main du joueur
float pourcent_selon_combi(Combi JEU){
    float pourcentage_mise=0;

    switch(JEU){
    case QUINTE_FLUSH:
        pourcentage_mise += 0.95;
        break;
    case CARRE:
        pourcentage_mise += 0.9;
        break;
    case FULL:
        pourcentage_mise += 0.8;
        break;
    case COULEUR:
        pourcentage_mise += 0.7;
        break;
    case SUITE:
        pourcentage_mise += 0.6;
        break;
    case BRELAN:
        pourcentage_mise += 0.5;
        break;
    case DOUBLE_PAIRE:
        pourcentage_mise += 0.4;
        break;
    case PAIRE:
        pourcentage_mise += 0.3;
        break;
    case BACKDOOR:
        pourcentage_mise += 0.1;
        break;
    }

    return pourcentage_mise;
}

// Rend un pourcentage selon jeu du joueur
float pourcentage_de_base_jeu(Joueur *grp,int indice,int phase){
    return pourcent_selon_combi(jeu_joueur_selon_phase(grp,indice,phase));
}

// Petit fonction aléatoire pour les chiffres décimaux [min,max[
float fonction_aleatoire_float(float min, float max){
    return (float)rand()/RAND_MAX*(max-min)+min;
}

// Fonction qui regarde si val € [born_inf;born_sup]
int est_dans_intervalle(float val, float born_inf, float born_sup){
	return (val>=born_inf && val<=born_sup)? OUI : NON ;
}

// Retourne le choix de l'IA au tour de mise
char choix_de_IA(Joueur *grp, Joueur *IA, Pot *pot, int indice, int phase){
    char choix='z'; int nb_outs=0, val_a_miser=(pot->dern_mise-IA->dern_mise);
    float cote_carte=0, cote_pot=0, choix_final=0, pourcentage_de_jouer=0, pourcentage_min_relance=0;

    choix_final              = fonction_aleatoire_float(0,1);

    nb_outs                  = (phase==PRE_FLOP || phase==RIVER)? 0 : calcule_nb_outs(grp,indice,phase);
    pourcentage_de_jouer     = (phase==PRE_FLOP)? pre_flop_pourcentage(IA) : pourcentage_de_base_jeu(grp,indice,phase);
    cote_pot                 = pourcentage_perte_pot(val_a_miser,IA->argent,pot->valeur);
    cote_carte               = pourcentage_outs(nb_outs,phase);

    pourcentage_de_jouer     += (faut_il_suivre(cote_pot,cote_carte))? cote_carte*1.5 : cote_carte;
    pourcentage_de_jouer     += ((1-pourcentage_de_jouer)*IA->parm.bluff);
    pourcentage_min_relance  = fonction_aleatoire_float(pourcentage_de_jouer*IA->parm.passivite,pourcentage_de_jouer);

    if     (est_dans_intervalle(choix_final,0,pourcentage_min_relance))                     choix = 's';
    else if(est_dans_intervalle(choix_final,pourcentage_min_relance,pourcentage_de_jouer))  choix = 'r';
    else                                                                                    choix = 'c';

    // Force le suivi de l'IA quand il peut checker (plus logique tres peu de personne se couche quand ca check)
    if(peux_parler(IA,pot)) choix = 's';

    return choix;
}

 // Change les parametre de l'IA à l'indice
void nouve_parm_ia(Joueur *groupe, int indice, float nouv_bluff, float nouv_passivite){
    groupe[indice].parm.bluff       = nouv_bluff;
    groupe[indice].parm.passivite   = nouv_passivite;
}

/** Fonction de test IA ***************************************************************************************************************************/
// Imprime la fiche du joueur a la suite du fichier
void imprimer_fiche_ia(FILE *fic, Joueur *ia){
	fprintf(fic,"Carte1:       %3d %c\n", (ia->main[0])/4, ia->main[0]%4 );
	fprintf(fic,"Carte2:       %3d %c\n", (ia->main[1])/4, ia->main[0]%4 );
	fprintf(fic,"Est_IA:       %3d\n",     ia->est_IA);
	fprintf(fic,"Passivite:    %.3f\n",    ia->parm.passivite);
	fprintf(fic,"Bluff:        %.3f\n",    ia->parm.bluff);
	fprintf(fic,"Valeur:       %I64u\n", ia->gagne.valeur);
    fprintf(fic,"\n\n");
}
