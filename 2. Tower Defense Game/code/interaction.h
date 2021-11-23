#ifndef _INTERACTION_
#define _INTERACTION_

/* INCLUDES */
#include "jeu.h"        /* Pour le type jeu_t dans interaction.h */
#include "fenetre.h"    /* Pour le type fenetre dans interaction.h */
#include "interface.h"  /* Pour le type interface*/
#include "socket.h"     /* Pour le type requete_tcp_t */

/* PROTOTYPES */
/**
 * Gestion des affichages
 * @param[out] interface (interface_t *)
 * @param[out] requete_recue (requete_tcp_t *)
 * @param[out] CASES_ORDI (int *)
 **/
void affichage(interface_t *interface, requete_tcp_t *requete_recue, int *CASES_ORDI);
/**
 * Affichage de la carte
 * @param[out] interface (interface_t *)
 * @param[out] jeu (jeu_t *)
 * @param[out] CASES_ORDI (int *)
 **/
void affichage_carte(interface_t *interface, jeu_t *jeu, int *CASES_ORDI);
/**
 * Affichage de la fenetre etat
 * @param[out] interface (interface_t *)
 * @param[out] jeu (jeu_t *)
 **/
void affichage_etat(interface_t *interface, jeu_t *jeu);
#endif