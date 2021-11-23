#ifndef _PARTIE_
#define _PARTIE_

/**
 * Ce fichier contient toutes les constantes structures utilisées 
 * pour la mise en place d'une partie dans le segment de mémoire, celles pour
 * les échanges de requêtes TCP entre le petit fils et son client et celles 
 * concernant les fichiers et scénarios
 */

/* INCLUDES */
#include "jeu.h" /* Pour le type jeu_t */

/* CONSTANTES */
/* Nombres max de parties, scenarios */
#define PARTIE_1 1 
#define PARTIE_2 2 
#define NB_MAX_PARTIE 2
/* Nombres max de cartes, scenarios contenus dans les fichiers */
#define NB_MAX_CARTE 3
#define NB_MAX_SCENARIO 3
#define NB_MAX_ENTREES 8
/* Nombres max de joueur dans partie */
#define NB_MAX_JOUEUR 4

/* TYPES REQUETES */
/* AVANT JEU */
/* TYPE REQUETE SERVEUR -> CLIENT */
#define CONNEXION_ETABLIE 1
#define CONNEXION_PARTIE 2
#define DEBUT_PARTIE 3
/* TYPE REQUETE CLIENT -> SERVEUR */
#define CREER_PARTIE 0
#define AJOUTER_PARTIE_1 1
#define AJOUTER_PARTIE_2 2

/* PENDANT JEU */
/* TYPE REQUETE CLIENT -> SERVEUR */
#define FREEZE 1
#define UNFREEZE 2
#define PLACER_TOUR 3
#define ATTAQUE 4
/* TYPE REQUETE SERVEUR -> CLIENT */
#define UPDATE_JEU 1
#define FIN_JOUEUR 2
#define FIN_PARTIE 3    


/* DONNEES INTIALISATION PARTIE */
#define VIE_MAX_JOUEUR 10
#define ARGENT_DEPART_JOUEUR 150
#define FREEZE_OFF 0
#define FREEZE_ON 1
#define UNFREEZE_OFF 0
#define UNFREEZE_ON 0

/* STRUCTURES */
/* Carte */
typedef struct{
    int id;
    char nom[256];
}carte_t;

/* Liste de carte */
typedef struct{
    carte_t cartes[NB_MAX_CARTE];
}liste_carte_t;

/* Scenario */
typedef struct{
    int id;
    char nom[256];
}scenario_t;

/* Liste de scenario */
typedef struct{
    scenario_t scenarios[NB_MAX_SCENARIO];
}liste_scenario_t;

/* Partie */
typedef struct{
    int id;
    int nb_joueur_courant; 
    char nom[256]; 
    carte_t carte;
    scenario_t scenario;  
    jeu_t jeu_j1; 
    jeu_t jeu_j2;
    jeu_t jeu_j3;
    jeu_t jeu_j4;
}partie_t;

/* Liste de partie */
typedef struct{
    partie_t parties[NB_MAX_PARTIE];
}liste_partie_t;

/* ligne scenario */
typedef struct{
    long temps;
    unsigned char type;
    char *msg;
    unsigned int unite;
    unsigned int freeze;
    unsigned int argent;
}ligne_scenario_t;

#endif