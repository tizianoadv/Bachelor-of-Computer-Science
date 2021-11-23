/**
 * udp_gestion_sem.c 
 * -> Fichier regroupant les fonctions utilisées pour la gestion du segment de mémoire partie udp.
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "udp_gestion_sem.h" /* Pour les prototypes de gestions sem udp */

/**
 * Vérification des informations (carte et scenario) du client 
 * @param[out] carte La carte
 * @param[out] scenario Le scenario
 * @return 1 si les informations sont correctes et 0 sinon
 */ 
int infos_partie_correctes(carte_t *carte, scenario_t *scenario){
    int i;
    int carte_ok = 0;
    int scenario_ok = 0;
    liste_carte_t liste_carte;
    liste_scenario_t liste_scenario;

    /* On récupère les cartes et scénarios */
    liste_carte = recuperer_liste_carte();
    liste_scenario = recuperer_liste_scenario();

    /* On compare les cartes */
    for( i=0 ; i < NB_MAX_CARTE ; i++ ){
        if(strcmp(liste_carte.cartes[i].nom, carte->nom) == 0)
            carte_ok++;
    }
    /* On compare les scenarios */
    for( i=0 ; i < NB_MAX_SCENARIO ; i++ ){
        if(strcmp(liste_scenario.scenarios[i].nom, scenario->nom) == 0)
            scenario_ok++;
    }

    return ( (carte_ok==1) && (scenario_ok==1) )? 1 : 0;
}


/**
 * Récuperation du nombre de partie en cours 
 * @param[out] partie1 La première partie
 * @param[out] partie2 La seconde partie
 * @return le nombre de parties
 */
int recuperer_nombre_parties(liste_sem_t *partie1, liste_sem_t *partie2){
    int nb_parties = 0;   
    int *id_partie = NULL;

    id_partie = partie1->id;
    if(*id_partie != 0 )
        /* Partie 1 existe */
        nb_parties++;
    
    id_partie = partie2->id;
    if(*id_partie != 0 )
        /* Partie 2 existe */
        nb_parties++;

    id_partie = NULL;

    return nb_parties;
}


/**
 * Récupération du numéro d'une partie existante 
 * @param[out] partie1 La première partie
 * @param[out] partie2 La seconde partie
 * @param[out] carte_p La carte que l'on souhaite
 * @param[out] scenario_p Le scénario que l'on veut
 * @return le numéro de la partie qui correspond à celle que l'on cherche
 */
int partie_existe(liste_sem_t *partie1, liste_sem_t *partie2, carte_t *carte_p, scenario_t *scenario_p){
    int num_partie_existante = 0;
    carte_t *carte = NULL;
    scenario_t *scenario = NULL;

    /* On compare carte_p et scenario_p avec la première partie */
    carte = partie1->carte;
    scenario = partie1->scenario;
    if( (strcmp(carte->nom, carte_p->nom) == 0) && (strcmp(scenario->nom, scenario_p->nom) == 0) )
        num_partie_existante = PARTIE_1;

    /* On compare carte_p et scenario_p avec la seconde partie */
    carte = partie2->carte;
    scenario = partie2->scenario;
    if( (strcmp(carte->nom, carte_p->nom) == 0) && (strcmp(scenario->nom, scenario_p->nom) == 0) )
        num_partie_existante = PARTIE_2;

    return num_partie_existante;
}


/**
 * Récupération du nombre de joueur en cours dans une partie
 * @param[out] partie1 La première partie
 * @param[out] partie2 La seconde partie
 * @param[out] num_partie Le numéro de la partie
 * @return le nombre de joueurs de la partie que l'on souhaite
 */
int recuperer_nombre_joueur_partie(liste_sem_t *partie1, liste_sem_t *partie2, int num_partie){
    int* nb_joueur = NULL;

    if( num_partie == PARTIE_1)
        nb_joueur = partie1->nb_joueur_courant;
    else
        nb_joueur = partie2->nb_joueur_courant;

    return *nb_joueur;
}

/**
 * Récupération de la liste des parties
 * @param[out] partie1 La première partie
 * @param[out] partie2 La seconde partie
 * @return une liste_partie composé des parties 1 et 2
 */
liste_partie_t recuperer_liste_partie(liste_sem_t *partie1, liste_sem_t *partie2){
    liste_partie_t retour;
    partie_t *tmp = NULL;

    /* On copie la Partie 1 dans la liste_partie */
    tmp = partie1->id;
    strcpy(retour.parties[0].nom, tmp->nom);
    strcpy(retour.parties[0].carte.nom, tmp->carte.nom);
    strcpy(retour.parties[0].scenario.nom, tmp->scenario.nom); 

    /* On copie la Partie 2 dans la liste_partie */
    tmp = partie2->id;
    strcpy(retour.parties[1].nom, tmp->nom);
    strcpy(retour.parties[1].carte.nom, tmp->carte.nom);
    strcpy(retour.parties[1].scenario.nom, tmp->scenario.nom);

    printf("nom partie 1 :%s\n", retour.parties[0].nom);
    printf("nom partie 2 :%s\n", retour.parties[1].nom);

    return retour;
}