#ifndef _TCP_CONNEXION_
#define _TCP_CONNEXION_

/* INCLUDES */
#include "sem.h"    /* Pour le type sem_t*/
#include "socket.h" /* Pour le type socket_t */

/* CONSTANTES */
#define NB_MAX_CONNEXION 4

/* PROTOTYPES */
/**
 * Demarrage du fils qui gère les connexion TCP
 * @param[out] pid_fils  Le pid du fils
 * @param[out] port_tcp  Le numéro du port tcp
 * @param[out] sem       Le segment de mémoire
 * @param[out] partie1   La première partie
 * @param[out] partie2   La seconde partie
 */
void creer_fils(int *pid_fils, int port_tcp, sem_t *sem, liste_sem_t *partie1, liste_sem_t *partie2);

/**
 * Demarrage le processus d'attente de connexion TCP
 * @param[out] port_tcp  Le numéro du port tcp
 * @param[out] sem       Le segment de mémoire
 * @param[out] partie1   La première partie
 * @param[out] partie2   La seconde partie
 */
void demarrer_socket_connexion(int port_tcp, sem_t *sem, liste_sem_t *partie1, liste_sem_t *partie2);

/**
 * Création des petits fils qui gère les communications TCP
 * @param[out] socket_connexion  La socket de connexion
 * @param[out] socket_communication  La socket de communication
 * @param[out] sem        Le segment de mémoire
 * @param[out] num_petit_fils Le numéro du petit fils
 * @param[out] partie1   La première partie
 * @param[out] partie2   La seconde partie
 */
void creer_petits_fils(socket_t *socket_connexion, socket_t *socket_communication, sem_t *sem, int num_petit_fils, liste_sem_t *partie1, liste_sem_t *partie2);

#endif