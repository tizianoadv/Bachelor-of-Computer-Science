/**
 * tcp_gestion_sem.c 
 * -> Fichier regroupant les fonctions utilisées pour la gestion du segment de mémoire partie tcp.
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "socket.h"             /* Pour les fonction de socket et le type requete_tcp_t*/
#include "tcp_gestion_sem.h"    /* Pour les prototypes */
#include "fichier.h"            /* Pour les prototypes des fonctions fichier */
#include "fenetre.h"            /* Pour le type fenetre_t */
#include "interface.h"          /* Pour les cases ordi et fort et adv */


/**
 * Gestion de la connexion à la partie
 * @param[out] requete_recue La requete tcp qui est recue
 * @param[out] partie1 La première partie
 * @param[out] partie2 La seconde partie
 * @param[out] id_p    Id de la partie
 * @param[out] id_j    Id du joueur
 * @param[out] nb_j_c  Le nombre de joueur courant 
 */
void gerer_connexion_partie(requete_tcp_t *recue, int *id_p, int *id_j, int *nb_j_c, liste_sem_t *partie1, liste_sem_t *partie2){

    switch(recue->id_partie){
        case CREER_PARTIE:
            /*Création de partie*/
            creer_partie(partie1, partie2, &recue->carte, &recue->scenario);
            *id_p = recuperer_nombre_partie(partie1,partie2);
            *id_j = 1;
            *nb_j_c = 1;
        break;
        case AJOUTER_PARTIE_1:
            /*Ajouter client à la partie 1*/
            *id_p = PARTIE_1;
            *id_j = recuperer_nombre_joueur(*id_p, partie1, partie2) + 1;
            ajouter_joueur(partie1);
        break;
        case AJOUTER_PARTIE_2:
            /*Ajouter client à la partie 2*/
            *id_p = PARTIE_2;
            *id_j = recuperer_nombre_joueur(*id_p, partie1, partie2) + 1;
            ajouter_joueur(partie2);
        break;
    }
}

/**
 * Gestion de la connexion à la partie
 * @param[out] partie1    La première partie
 * @param[out] partie2    La seconde partie
 * @param[out] carte_p    La carte que l'on souhaite
 * @param[out] scenario_p Le scenario que l'on souhaite 
 */
void creer_partie(liste_sem_t *partie1, liste_sem_t *partie2, carte_t *carte_p, scenario_t *scenario_p){
    int *id_partie1=0;
    int *id_partie2 = 0;
    char nom_partie[256] = {0};
    partie_t* partie_tmp = NULL;

    id_partie1 = partie1->id;
    id_partie2 = partie2->id;
    if(*id_partie1 == 0 && *id_partie2 == 0){
        /* Aucune partie n'existe */
        /* La partie est alors créée sur la partie 1 */
        partie_tmp = partie1->id;
        partie_tmp->id = 1;
        partie_tmp->nb_joueur_courant = 1;
        sprintf(nom_partie, "%s%d", "partie", 1);
        strcpy(partie_tmp->nom, nom_partie);
        memcpy(&partie_tmp->carte,carte_p,sizeof(carte_t));
        memcpy(&partie_tmp->scenario,scenario_p,sizeof(scenario_t));
        initialiser_jeu(&partie_tmp->jeu_j1, carte_p);
        initialiser_jeu(&partie_tmp->jeu_j2, carte_p);
        initialiser_jeu(&partie_tmp->jeu_j3, carte_p);
        initialiser_jeu(&partie_tmp->jeu_j4, carte_p);
    }
    else if(*id_partie1 == 1 && *id_partie2 == 0){
        /* La partie 1 existe */
        /* La partie 2 n'existe pas */
        /* La partie est alors créée sur la partie 2 */
        partie_tmp = partie2->id;
        partie_tmp->id = 2;
        partie_tmp->nb_joueur_courant = 1;
        sprintf(nom_partie, "%s%d", "partie", 2);
        strcpy(partie_tmp->nom, nom_partie);
        memcpy(&partie_tmp->carte,carte_p,sizeof(carte_t));
        memcpy(&partie_tmp->scenario,scenario_p,sizeof(scenario_t));
        initialiser_jeu(&partie_tmp->jeu_j1, carte_p);
        initialiser_jeu(&partie_tmp->jeu_j2, carte_p);
        initialiser_jeu(&partie_tmp->jeu_j3, carte_p);
        initialiser_jeu(&partie_tmp->jeu_j4, carte_p);
    }
    else if(*id_partie1 == 0 && *id_partie2 == 2){
        /* La partie 1 n'existe pas */
        /* La partie 2 existe */
        /* La partie est alors créée sur la partie 1 */
        partie_tmp = partie1->id;
        partie_tmp->id = 1;
        partie_tmp->nb_joueur_courant = 1;
        sprintf(nom_partie, "%s%d", "partie", 1);
        strcpy(partie_tmp->nom, nom_partie);
        memcpy(&partie_tmp->carte,carte_p,sizeof(carte_t));
        memcpy(&partie_tmp->scenario,scenario_p,sizeof(scenario_t));
        initialiser_jeu(&partie_tmp->jeu_j1, carte_p);
        initialiser_jeu(&partie_tmp->jeu_j2, carte_p);
        initialiser_jeu(&partie_tmp->jeu_j3, carte_p);
        initialiser_jeu(&partie_tmp->jeu_j4, carte_p);
    }   
}

/**
 * Initialise les jeux
 * @param[out] jeu      Le jeu
 * @param[out] carte_p  La carte 
 */
void initialiser_jeu(jeu_t *jeu, carte_t *carte_p){
    int i; 
    
    memcpy(jeu->carte, recuperer_infos_carte(carte_p->nom), sizeof(unsigned char)*225);
    jeu->vies = VIE_MAX_JOUEUR;
    for(i = 0; i<3; i++) 
        jeu->adv[i] = VIE_MAX_JOUEUR;

    jeu->argent = ARGENT_DEPART_JOUEUR;
    jeu->freeze = FREEZE_OFF;
    jeu->unfreeze = UNFREEZE_OFF;
}

/**
 * Ajoute un client à une partie
 * @param[out] partie La partie pour laquelle on ajoute un joueur
 */
void ajouter_joueur(liste_sem_t *partie){
    partie_t *partie_tmp = NULL;
    partie_tmp = partie->id;
    /* On ajoute un joueur */
    (partie_tmp->nb_joueur_courant)++;
}

/**
 * Recupération du nombre de joueur courant d'une partie
 * @param[out] partie1    La première partie
 * @param[out] partie2    La seconde partie
 * @param[out] id_partie  Le numéro de la partie
 * @return le nombre de joueurs dans une partie
 */
int recuperer_nombre_joueur(int id_partie, liste_sem_t *partie1, liste_sem_t *partie2){
    partie_t *tmp = NULL;
    if( id_partie == PARTIE_1)
        tmp = partie1->id;
    else    
        tmp = partie2->id;
    return tmp->nb_joueur_courant;     
}

/**
 * Recupération du nombre de partie courante
 * @param[out] partie1    La première partie
 * @param[out] partie2    La seconde partie
 * @return le nombre de parties qui existent
 */
int recuperer_nombre_partie(liste_sem_t *partie1, liste_sem_t *partie2){
    int nb_partie_courante=0;
    int *tmp1 = NULL;
    int *tmp2 = NULL;

    tmp1 = partie1->id;
    tmp2 = partie2->id;
    if( *tmp1 == 0 && *tmp2 == 0)
        /* Aucune partie existe */
        nb_partie_courante=0;
    else if( *tmp1 == 1 && *tmp2 == 0 )
        /* Partie 1 existe */
        nb_partie_courante=1;
    else if( *tmp1 == 0 && *tmp2 == 2 )
        /* Partie 2 existe */
        nb_partie_courante=1;
    else
        /* Les 2 parties existent */
        nb_partie_courante=2;

    return nb_partie_courante;
}

/**
 * Recuperer une partie en fonction de l'id de la partie
 * @param[out] partie1    La première partie
 * @param[out] partie2    La seconde partie
 * @param[out] id_partie  Le numéro de la partie
 * @return la partie que l'on souhaite
 */
partie_t* recuperer_partie(int id_partie, liste_sem_t* partie1, liste_sem_t* partie2){
    
    if( id_partie == PARTIE_1 )
        return partie1->id;
    else
        return partie2->id;
}

/**
 * Recuperer un jeu en fonction de l'id du joueur
 * @param[out] partie   La première partie
 * @param[out] id_joueur  Le numéro de la partie
 * @return le jeu
 */
jeu_t* recuperer_jeu_joueur(int id_joueur, partie_t *partie){
    jeu_t *tmp = NULL;
    switch(id_joueur){
        case 1:
            tmp = &partie->jeu_j1;
        break;
        case 2:
            tmp = &partie->jeu_j2;
        break;
        case 3:
            tmp = &partie->jeu_j3;
        break;
        case 4:
            tmp = &partie->jeu_j4;
        break;
    }
    return tmp;
}

/**
 * Initialise une carte en fonction de l'id du joueur
 * @param[out] partie     La partie
 * @param[out] id_joueur  Le numéro du joueur dans la partie
 */
void initialiser_carte(int id_joueur, partie_t *partie){
    int i=0,j=0;
    switch(id_joueur){
        case 1:
            /* Joueur 1 */
            /* Les autre joueurs sont 2, 3 et 4 */
            for( i=0 ; i < 15 ; i++ ){
                for( j=0 ; j < 15 ; j++ ){
                    if( partie->jeu_j1.carte[i][j] > 0 && partie->jeu_j1.carte[i][j] <= 4)
                        /* On modifie la carte */
                        partie->jeu_j1.carte[i][j] +=1;
                }
            }
        break;
        case 2:
            /* Joueur 2 */
            /* Les autre joueurs sont 1, 3 et 4 */
            for( i=0 ; i < 15 ; i++ ){
                for( j=0 ; j < 15 ; j++ ){
                    if( partie->jeu_j2.carte[i][j] > 1 && partie->jeu_j2.carte[i][j] <= 4)
                        /* On modifie la carte */
                        partie->jeu_j2.carte[i][j] +=1;
                }
            }
        break;
        case 3:
            /* Joueur 3 */
            /* Les autre joueurs sont 1, 2 et 4 */
            for( i=0 ; i < 15 ; i++ ){
                for( j=0 ; j < 15 ; j++ ){
                    if( partie->jeu_j3.carte[i][j] > 2 && partie->jeu_j3.carte[i][j] <= 4)
                        /* On modifie la carte */
                        partie->jeu_j3.carte[i][j] +=1;
                }
            }
        break;
        case 4:
            /* Joueur 4 */
            /* Les autre joueurs sont 1, 2 et 3 */
            for( i=0 ; i < 15 ; i++ ){
                for( j=0 ; j < 15 ; j++ ){
                    if( partie->jeu_j4.carte[i][j] == 4 )
                        /* On modifie la carte */
                        partie->jeu_j4.carte[i][j] +=1;
                }
            }
        break;
    }
}
