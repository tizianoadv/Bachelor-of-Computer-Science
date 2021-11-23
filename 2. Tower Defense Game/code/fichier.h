#ifndef _FICHIER_
#define _FICHIER_

/* INCLUDES */
#include <fcntl.h>  /* Pour off_t*/
#include <dirent.h> /*Pour le type DIR* */
#include "partie.h" /*Pour les types liste_carte_t, liste_scenario_t */

/* PROTOTYPES */
/**
 * Recupère la liste des cartes 
 * @return une liste carte (liste_carte_t)
 **/
liste_carte_t recuperer_liste_carte();

/**
 * Recupère la liste des scenarios 
 * @return une liste scenario (liste_scenario_t)
 **/
liste_scenario_t recuperer_liste_scenario();

/**
 * Récupération des informations des fichiers binaires des cartes
 * @param[out] nom_carte (char*)
 * @return*chaine (unsigned char*)
 **/
unsigned char* recuperer_infos_carte(char *nom_carte);

/**
 * Ouvre un scénario en lecture et place la tete lecture au bon endroit
 * @param[out] nom_scenario (char*)
 * @return fd (int)
 **/
int ouvrir_scenario(char *nom_scenario);

/**
 * Lecture du scenario ligne par ligne
 * @param[out] fd (int)
 * @param[out] total_lu (ssize_t *)
 * @param[out] ligne (ligne_scenario_t *)
 **/
void lire_scenario(int fd, ssize_t *total_lu, ligne_scenario_t *ligne);

/**
 * Ouvrir fichier en lecture
 * @param[out] pathname (char *)
 * @return fd (int)
 **/
int ouvrir_fichier_lecture(char *pathname);

/**
 * Mise de la tete de lecture au début du fichier
 * @param[out] fd (int)
 **/
void tete_lecture_debut(int fd);

/**
 * Sauter la description
 * @param[out] fd (int)
 * @param[out] taille_saut (off_t)
 **/
void sauter_description(int fd, off_t taille_saut);

/**
 * Lecture d'une case
 * @param[out] fd (int)
 * @param[out] buffer (unsigned char *)
 **/
void lire_case(int fd, unsigned char *buffer);

/**
 * Lecture de la taille de la description
 * @param[out] fd (int)
 * @param[out] taille_description (off_t *)
 **/
void lire_taille(int fd, off_t *taille_description);

/**
 * Lecture du temps 
 * @param[out] fd (int)
 * @param[out] lu (ssize_t *)
 * @return temp (long)
 **/
long lire_temps(int fd, ssize_t *lu);

/**
 * Lecture du type 
 * @param[out] fd (int)
 * @param[out] lu (ssize_t *)
 * @return type (unsigned char)
 **/
unsigned char lire_type(int fd, ssize_t *lu);

/**
 * Lecture d'une chaine 
 * @param[out] fd (int)
 * @param[out] lu (ssize_t *)
 * @return chaine (char*)
 **/
char* lire_chaine(int fd, ssize_t *lu);

/**
 * Lecture d'une donnée
 * @param[out] fd (int)
 * @param[out] lu (ssize_t *)
 * @return donnée (unsigned int)
 **/
unsigned int lire_data(int fd, ssize_t *lu);
#endif