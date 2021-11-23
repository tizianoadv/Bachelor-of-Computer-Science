/**
 * liste.c 
 * -> Gestion des fonctions de gestions de liste
 * -> Les listes servent à lire/écrire dans la segment de mémoire partagée
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>  /* Pour memset */

#include "liste.h"   /* Pour les prototypes */
#include "partie.h"  /* Pour les types carte_t, scenario_t, partie_t */

/**
 * Création de la liste de partie
 * @return liste
 */
liste_sem_t creer_liste(){
    liste_sem_t retour;
    retour.id = NULL;
    retour.nb_joueur_courant = NULL;
    retour.nom = NULL;
    retour.carte = NULL;
    retour.scenario = NULL;
    retour.jeu_j1 = NULL;
    retour.jeu_j2 = NULL;
    retour.jeu_j3 = NULL;
    retour.jeu_j4 = NULL;
    return retour;
}

/**
 * Fonction permettant de creer 2 elements dans la liste et de les attacher au segment 
 * @param[out] liste (liste)
 * @param[out] sem (segment de mémoire) 
 * @param[out] num_partie (numero de partie) 
 */
void attacher_liste_sem(liste_sem_t* liste, sem_t *sem, int num_partie){
    partie_t *partie = NULL;

    if( num_partie == PARTIE_1){
        partie = sem->addr;
        liste->id                = &partie->id;
        liste->nb_joueur_courant = &partie->nb_joueur_courant;
        liste->nom               = &partie->nom;
        liste->carte             = &partie->carte;
        liste->scenario          = &partie->scenario;
        liste->jeu_j1            = &partie->jeu_j1;
        liste->jeu_j2            = &partie->jeu_j2;
        liste->jeu_j3            = &partie->jeu_j3;
        liste->jeu_j4            = &partie->jeu_j4;
    }else{
        partie = sem->addr;
        partie++;
        liste->id                = &partie->id;
        liste->nb_joueur_courant = &partie->nb_joueur_courant;
        liste->nom               = &partie->nom;
        liste->carte             = &partie->carte;
        liste->scenario          = &partie->scenario;
        liste->jeu_j1            = &partie->jeu_j1;
        liste->jeu_j2            = &partie->jeu_j2;
        liste->jeu_j3            = &partie->jeu_j3;
        liste->jeu_j4            = &partie->jeu_j4;
    }
}

/**
 * Fonction qui initialise les parties dans le segment de mémoire
 * @param[out] liste (liste)
 */
void initialiser_parties(liste_sem_t *liste){
    partie_t *partie = NULL;  
    partie = liste->id;
    memset(partie, 0, sizeof(partie_t));
}

/**
 * Suppression de tous les elements de la liste
 * @param[out] liste la liste partie
 */
void supprimer_liste(liste_sem_t* liste){
    liste->id = NULL;
    liste->nb_joueur_courant = NULL;
    liste->nom = NULL;
    liste->carte = NULL;
    liste->scenario = NULL;
    liste->jeu_j1 = NULL;
    liste->jeu_j2 = NULL;
    liste->jeu_j3 = NULL;
    liste->jeu_j4 = NULL;
}
