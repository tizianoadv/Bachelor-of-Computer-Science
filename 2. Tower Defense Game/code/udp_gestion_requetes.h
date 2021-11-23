#ifndef _UDP_GESTION_REQUETES_ 
#define _UDP_GESTION_REQUETES_

/* INCLUDES */
#include "socket.h"      /* Pour le type requete_udp_t */
#include "liste.h"       /* Pour le type liste_sem_t */
                        
#include "jeu.h"
#include "fenetre.h"
#include "interface.h"   /* Pour le type interface_t */

/* CONSTANTES */
/* ID de requêtes */
#define LISTE_CARTE 1
#define LISTE_SCENARIO 2
#define DEMARRAGE_PARTIE 3
#define LISTE_PARTIE_DEMAREES 4
#define ERREUR_REQUETE 5
#define NB_MAX_PARTIE_ATTEINT 6

/* PROTOTYPES */
/* PARTIE SERVEUR */
/**
 * Traitement des requêtes UDP encvoyées par un client et des réponses envoyées
 * @param[out] requete_recue La requete qui est recue
 * @param[out] requete_envoie La requete qu'on envoie
 * @param[out] interface L'interface ncurses
 */
void serveur_gestion_udp(requete_udp_t *requete_recue, requete_udp_t *requete_envoie, liste_sem_t *partie1, liste_sem_t *partie2, int num_port_tcp);

/* PARTIE CLIENT */
/**
 * Menu temporaire pour le client
 * @param[out] requete_recue La requete qui est recue
 * @param[out] requete_envoie La requete qu'on envoie
 * @param[out] interface L'interface ncurses
 */
void menu(requete_udp_t *requete_recue, requete_udp_t *requete_envoie, interface_t *interface);

/**
 * Traitement des requêtes UDP encvoyées par le serveur et des réponses envoyées
 * @param[out] requete_recue La requete qui est recue
 * @param[out] requete_envoie La requete qu'on envoie
 * @param[out] interface L'interface ncurses
 */
void client_gestion_udp(requete_udp_t *requete_recue, requete_udp_t *requete_envoie, interface_t *interface);

#endif