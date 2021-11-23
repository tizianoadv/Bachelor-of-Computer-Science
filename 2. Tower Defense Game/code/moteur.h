#ifndef _MOTEUR_
#define _MOTEUR_

/* INCLUDES */
#include "socket.h"     /* Pour requete tcp_t */
#include "jeu.h"        /* Pour le type jeu_t */
#include "partie.h"     /* Pour le type partie_t */

/* PROTOTYPES */
/**
 * Modification des informations de la partie en fonctione du scénario
 * @param[out] socket_communication (socket_t *)
 * @param[out] partie (partie_t *)
 * @param[out] jeu_joueur (jeu_t *)
 **/
void gerer_scenario(socket_t *socket_communication, partie_t *partie, jeu_t *jeu_joueur);

/**
 * Ajout nouvel unité 
 * @param[out] jeu (jeu_t *)
 * @param[out] scenario (ligne_scenario_t *)
 * @param[out] CASE_ORDI_I (int *)
 * @param[out] CASE_ORDI_J (int *)
 **/
void ajouter_unite(jeu_t *jeu, ligne_scenario_t *scenario, int CASE_ORDI_I, int CASE_ORDI_J);

/**
 * Déplacement des unités présentes sur la matrice
 * @param[out] jeu (jeu_t *)
 * @param[out] unite (int)
 **/
void deplacer_unite(jeu_t *jeu, int unite);

/**
 *  Recuperation la case de l'ordi
 * @param[out] jeu (jeu_t *)
 * @param[out] CASE_ORDI_I (int *)
 * @param[out] CASE_ORDI_J (int *)
 **/
void recuperer_case_ordi(jeu_t *jeu, int *CASE_ORDI_I, int *CASE_ORDI_J);

/**
 * Vérification des position d'une unité dans la matrice
 * @param[out] jeu (jeu_t *)
 * @param[out] i (int)
 * @param[out] ij(int)
 * @param[out] tab (unsigned char*)
 **/
void verifier_position(jeu_t *jeu, int i, int j, unsigned char* tab);

int chercher(int pos[100], int x, int y);

/**
 * Prépare la requete pour l'envoie du jeu du jour 
 * @param[out] partie (partie_t *)
 * @param[out] jeu (jeu_t *)
 * @param[out] scenario_l (ligne_scenario_t *)
 * @param[out] requete_envoie (requete_tcp_t *)
 **/
void preparer_requete_jeu(partie_t *partie, jeu_t *jeu, ligne_scenario_t *scenario_l, requete_tcp_t *requete_envoie);
#endif
