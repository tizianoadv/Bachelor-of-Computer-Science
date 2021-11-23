#ifndef _TCP_COMMUNICATION_
#define _TCP_COMMUNICATION_

/* INCLUDES */
#include "partie.h"     /* Pour les structures et constantes de partie */
#include "socket.h"     /* Pour le type socket_t */
#include "sem.h"        /* Pour le type sem_t */
#include "liste.h"      /* Pour le type liste_sem_t */
/* Pour le type interface_t */
#include "jeu.h"
#include "fenetre.h"
#include "interface.h" 

/* PROTOTYPES */
/* PARTIE PETIT FILS */
/* Communication avec un client */
/**
 * Gestion de la communication avec un client
 * @param[out] socket_communication (socket_t *)
 * @param[out] sem (sem_t *)
 * @param[out] partie1 (liste_sem_t *)
 * @param[out] partie2 (liste_sem_t *)
 **/
void communiquer_avec_client(socket_t *socket_communication, sem_t *sem, liste_sem_t *partie1, liste_sem_t *partie2);


/* PARTIE CLIENT */
/**
 * Gestion de la communication avec un petit fils pour la partie début partie
 * @param[out] socket_communication (socket_t *)
 * @param[out] interface (interface_t *)
 * @param[out] recue_udp (requete_udp_t *)
 * @param[out] requete_recue (requete_tcp_t *)
 **/
void connecter_partie(socket_t *socket_communication, interface_t *interface, requete_udp_t *recue_udp, requete_tcp_t *requete_recue, int *CASES_ORDI);

/**
 * Gestion de la communication avec un petit fils pour les parties pendant et fin partie
 * @param[out] socket_communication (socket_t *)
 * @param[out] requete_recue (requete_tcp_t *)
 **/
void receptionner_info_jeu(socket_t *socket_communication, requete_tcp_t *requete_recue);

#endif