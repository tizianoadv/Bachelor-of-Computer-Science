#ifndef _LISTE_
#define _LISTE_

/* INCLUDES */
#include "sem.h"     /* Pour le type sem_t */   

/* CONSTANTES */
/* Le nombre max d'élement dans une liste */
#define PARTIE_1 1
#define PARTIE_2 2

/* STRUCTURES */
/* Structure d'une liste */
typedef struct liste_sem{
    void *id;
    void *nb_joueur_courant;
    void *nom;
    void *carte;
    void *scenario;
    void *jeu_j1;
    void *jeu_j2;
    void *jeu_j3;
    void *jeu_j4;
}liste_sem_t;


/* PROTOTYPES */
/**
 * Création de la liste partie
 * @return liste
 */
liste_sem_t creer_liste();

/**
 * Fonction permettant de creer 2 elements dans la liste et de les attacher au segment 
 * @param[out] liste (liste)
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
 * Supprime tous les elements de la liste partie
 * @param[out] liste la liste partie
 */
void supprimer_liste(liste_sem_t* liste);

#endif