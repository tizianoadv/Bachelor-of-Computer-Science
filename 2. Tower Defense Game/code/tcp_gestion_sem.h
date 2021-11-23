#ifndef _TCP_GESTION_SEM_
#define _TCP_GESTION_SEM_

/* INCLUDES */
#include "liste.h"      /* Pour le type liste_sem_t */
#include "partie.h"     /* Pour les types carte_t scenario_t */
#include "socket.h"     /* Pour le type requet_tcp_t */

/**
 * Gestion de la connexion à la partie
 * @param[out] requete_recue La requete tcp qui est recue
 * @param[out] partie1 La première partie
 * @param[out] partie2 La seconde partie
 * @param[out] id_p    Id de la partie
 * @param[out] id_j    Id du joueur
 * @param[out] nb_j_c  Le nombre de joueur courant 
 */
void gerer_connexion_partie(requete_tcp_t *recue, int *id_p, int *id_j, int *nb_j_c, liste_sem_t *partie1, liste_sem_t *partie2);

/**
 * Gestion de la connexion à la partie
 * @param[out] partie1    La première partie
 * @param[out] partie2    La seconde partie
 * @param[out] carte_p    La carte que l'on souhaite
 * @param[out] scenario_p Le scenario que l'on souhaite 
 */
void creer_partie(liste_sem_t *partie1, liste_sem_t *partie2, carte_t *carte_p, scenario_t *scenario_p);

/**
 * Initialise les jeux
 * @param[out] jeu      Le jeu
 * @param[out] carte_p  La carte 
 */
void initialiser_jeu(jeu_t *jeu, carte_t *carte_p);

/**
 * Ajoute un client à une partie
 * @param[out] partie La partie pour laquelle on ajoute un joueur
 */
void ajouter_joueur(liste_sem_t *partie);

/**
 * Recupération du nombre de joueur courant d'une partie
 * @param[out] partie1    La première partie
 * @param[out] partie2    La seconde partie
 * @param[out] id_partie  Le numéro de la partie
 * @return le nombre de joueurs dans une partie
 */
int recuperer_nombre_joueur(int id_partie, liste_sem_t *partie1, liste_sem_t *partie2);

/**
 * Recupération du nombre de partie courante
 * @param[out] partie1    La première partie
 * @param[out] partie2    La seconde partie
 * @return le nombre de parties qui existent
 */
int recuperer_nombre_partie(liste_sem_t *partie1, liste_sem_t *partie2);

/**
 * Recuperer une partie en fonction de l'id de la partie
 * @param[out] partie1    La première partie
 * @param[out] partie2    La seconde partie
 * @param[out] id_partie  Le numéro de la partie
 * @return la partie que l'on souhaite
 */
partie_t* recuperer_partie(int id_partie, liste_sem_t* partie1, liste_sem_t* partie2);

/**
 * Recuperer un jeu en fonction de l'id du joueur
 * @param[out] partie   La première partie
 * @param[out] id_joueur  Le numéro de la partie
 * @return le jeu
 */
jeu_t* recuperer_jeu_joueur(int id_joueur, partie_t *partie);

/**
 * Initialise une carte en fonction de l'id du joueur
 * @param[out] partie     La partie
 * @param[out] id_joueur  Le numéro du joueur dans la partie
 */
void initialiser_carte(int id_joueur, partie_t *partie);

#endif