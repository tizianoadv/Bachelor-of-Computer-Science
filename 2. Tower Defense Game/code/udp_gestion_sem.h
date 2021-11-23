#ifndef _UDP_GESTION_SEM_
#define _UDP_GESTION_SEM_

/* INCLUDES */
#include "fichier.h"    /* Pour les types : carte_t, scenario_t, liste_carte_t, liste_scenario_t */
                        /* Pour les defines : NB_MAX_CARTE et NB_MAX_SCENARIO */
                        /* Pour les prototypes de recuperer_liste_carte_t et recuperer liste_ scenario_t */
#include "liste.h"      /* Pour le type liste_sem_t */
#include "partie.h"     /* Pour le type liste_partie_t */ 

/* PROTOTYPES */
/**
 * Fonction permettant de creer 2 elements dans la liste et de les attacher au segment 
 * @param[out] liste (liste)
 * @param[out] num_partie (le numéro de la partie)
 * @param[out] sem (segment de mémoire) 
 * @param[out] num_partie (numero de partie) 
 */
void attacher_liste_sem(liste_sem_t* liste, sem_t *sem, int num_partie);

/**
 * Fonction qui initialise les parties dans le segment de mémoire
 * @param[out] liste (liste)
 */
void initialiser_parties(liste_sem_t *liste);

/**
 * Vérification des informations (carte et scenario) du client 
 * @param[out] carte La carte
 * @param[out] scenario Le scenario
 * @return 1 si les informations sont correctes et 0 sinon
 */ 
int infos_partie_correctes(carte_t *carte, scenario_t *scenario);

/**
 * Récuperation du nombre de partie en cours 
 * @param[out] partie1 La première partie
 * @param[out] partie2 La seconde partie
 * @return le nombre de parties
 */
int recuperer_nombre_parties(liste_sem_t *partie1, liste_sem_t *partie2);

/**
 * Récupération du numéro d'une partie existante 
 * @param[out] partie1 La première partie
 * @param[out] partie2 La seconde partie
 * @param[out] carte_p La carte que l'on souhaite
 * @param[out] scenario_p Le scénario que l'on veut
 * @return le numéro de la partie qui correspond à celle que l'on cherche
 */
int partie_existe(liste_sem_t *partie1, liste_sem_t *partie2, carte_t *carte_p, scenario_t *scenario_p);

/**
 * Récupération du nombre de joueur en cours dans une partie
 * @param[out] partie1 La première partie
 * @param[out] partie2 La seconde partie
 * @param[out] num_partie Le numéro de la partie
 * @return le nombre de joueurs de la partie que l'on souhaite
 */
int recuperer_nombre_joueur_partie(liste_sem_t *partie1, liste_sem_t *partie2, int num_partie);

/**
 * Récupération de la liste des parties
 * @param[out] partie1 La première partie
 * @param[out] partie2 La seconde partie
 * @return une liste_partie composé des parties 1 et 2
 */
liste_partie_t recuperer_liste_partie(liste_sem_t *partie1, liste_sem_t *partie2);

#endif