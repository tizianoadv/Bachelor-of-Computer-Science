/**
 * tcp_communication.c 
 * ->Partie petit fils
 *      -> Gestions des requêtes envoies
 *      -> Traitement des requêtes reçeus
 *      -> Gestion du scenério
 * ->Partie client
 *      -> Gestions des requêtes envoies
 *      -> Traitement des requêtes reçeus
 *      -> Affichage sur l'interface
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>             /* Pour memset, strcpy */
#include <unistd.h>             /* pour sleep */

#include "tcp_communication.h"  /* Pour les prototypes */
#include "fichier.h"            /* Pour les fonctions de gestion de fichier */
#include "ncurses.h"            /* Pour les fonction de gestion d'interface */
#include "tcp_gestion_sem.h"    /* Pour les fonstions de gestion de sem */
#include "moteur.h"             /* Pour le type ligne_scenario_t et les fonctions du moteur du jeu */


/*** PARTIE PETIT FILS ***/
/**
 * Gestion de la communication avec un client
 * @param[out] socket_communication (socket_t *)
 * @param[out] sem (sem_t *)
 * @param[out] partie1 (liste_sem_t *)
 * @param[out] partie2 (liste_sem_t *)
 **/
void communiquer_avec_client(socket_t *socket_communication, sem_t *sem, liste_sem_t *partie1, liste_sem_t *partie2){
    
    int id_partie=0;
    int id_joueur=0;
    int nb_joueur_courant = 0;
    requete_tcp_t requete_envoie;
    requete_tcp_t requete_recue;
    jeu_t *jeu_joueur = NULL;
    partie_t *partie = NULL;

    /*Initialisation des requêtes*/
    memset(&requete_envoie,0, sizeof(requete_tcp_t));
    memset(&requete_recue,0, sizeof(requete_tcp_t));

    /* Envoie requete : CONNEXION_ETABLIE */
    requete_envoie.id = CONNEXION_ETABLIE;
    envoyer_requete_tcp(socket_communication, &requete_envoie);

    /* Reception requete : CONNEXION_PARTIE */
    receptionner_requete_tcp(socket_communication, &requete_recue);
    gerer_connexion_partie(&requete_recue,&id_partie,&id_joueur,&nb_joueur_courant,partie1,partie2);
    /* Récupération de la partie en cours */
    partie = recuperer_partie(id_partie, partie1, partie2);
    jeu_joueur = recuperer_jeu_joueur(id_joueur, partie);
    
    /* PREPARATION DEBUT PARTIE */
    /*Attente du nombre max de joueur de la partie*/
    while( nb_joueur_courant < NB_MAX_JOUEUR ){
        nb_joueur_courant = recuperer_nombre_joueur(id_partie, partie1, partie2);
        printf("Nombre joueur actuel : %d\n", nb_joueur_courant);
        sleep(3);
    }
    /* Initialisation d'une carte */
    initialiser_carte(id_joueur, partie);
    
    /* Envoie requete : DEBUT_PARTIE */
    requete_envoie.id = DEBUT_PARTIE;
    memcpy(&requete_envoie.jeu, jeu_joueur, sizeof(jeu_t));
    envoyer_requete_tcp(socket_communication, &requete_envoie);
    sleep(nb_joueur_courant + nb_joueur_courant);

    /* COMMUNICATION PENDANT LE JEU*/
    gerer_scenario(socket_communication, partie, jeu_joueur);

    /* Envoie requete : FIN_JOUEUR */
    sleep(5);
    memset(&requete_envoie, 0, sizeof(requete_tcp_t));
    requete_envoie.id = FIN_JOUEUR;
    envoyer_requete_tcp(socket_communication, &requete_envoie);

    /* ATTENTE FIN DES AUTRES JOUEURS */
    while( nb_joueur_courant > 0 ){
        sleep(1);
        printf("Attente fin joueurs \n");
        nb_joueur_courant = recuperer_nombre_joueur(id_partie, partie1, partie2);
    }
    
    /* Envoie requete : FIN_PARTIE */
    sleep(5);
    memset(&requete_envoie, 0, sizeof(requete_tcp_t));
    requete_envoie.id = FIN_PARTIE;
    envoyer_requete_tcp(socket_communication, &requete_envoie);
}

/*** PARTIE CLIENT ***/
/**
 * Gestion de la communication avec un petit fils pour la partie début partie
 * @param[out] socket_communication (socket_t *)
 * @param[out] interface (interface_t *)
 * @param[out] recue_udp (requete_udp_t *)
 * @param[out] requete_recue (requete_tcp_t *)
 **/
void connecter_partie(socket_t *socket_communication, interface_t *interface, requete_udp_t *recue_udp, requete_tcp_t *requete_recue, int *CASES_ORDI){
    requete_tcp_t requete_envoie;

    /*AFFICHAGE CONNEXION PARTIE*/
    wclear(interface->infos->interieur);
    wprintw(interface->infos->interieur, "\t\t* CONNEXION PARTIE *\n");
    wrefresh(interface->infos->interieur);

    /*ATTENTE CONNEXION ETABLIE */
    while( requete_recue->id != CONNEXION_ETABLIE ){
        sleep(3);
        receptionner_requete_tcp(socket_communication, requete_recue);
    }
    wprintw(interface->infos->interieur, "Connexion au serveur établie\n");
    wrefresh(interface->infos->interieur);

    /* REQUETE CONNEXION ETABLIE  */
    requete_envoie.id = CONNEXION_PARTIE;
    requete_envoie.id_partie = recue_udp->num_partie;
    requete_envoie.carte.id = recue_udp->carte.id;
    requete_envoie.scenario.id = recue_udp->scenario.id;
    strcpy(requete_envoie.carte.nom, recue_udp->carte.nom);
    strcpy(requete_envoie.scenario.nom, recue_udp->scenario.nom);
    envoyer_requete_tcp(socket_communication, &requete_envoie);
    wprintw(interface->infos->interieur, "Attente des autres joueurs ...\n");
    wrefresh(interface->infos->interieur);

    /* ATTENTE DES AUTRES JOUEURS */
    while( requete_recue->id != DEBUT_PARTIE ){
        receptionner_requete_tcp(socket_communication, requete_recue);
    }
    wprintw(interface->infos->interieur, "La partie va commencer !\n");
    wrefresh(interface->infos->interieur);
    sleep(1);
    wclear(interface->infos->interieur);
    wprintw(interface->infos->interieur, "\t\t* PARTIE EN COURS *");
    wrefresh(interface->infos->interieur);
    interface = interface_creer(&requete_recue->jeu);

    /* RECUPERATION DE LA CASE ORDI */
    recuperer_case_ordi(&requete_recue->jeu, &CASES_ORDI[0], &CASES_ORDI[1]);
}

/**
 * Gestion de la communication avec un petit fils pour les parties pendant et fin partie
 * @param[out] socket_communication (socket_t *)
 * @param[out] requete_recue (requete_tcp_t *)
 **/
void receptionner_info_jeu(socket_t *socket_communication, requete_tcp_t *requete_recue){
    /*RECPETION DES REQUETES TCP*/
    memset(requete_recue,0,sizeof(requete_tcp_t));
    receptionner_requete_tcp(socket_communication, requete_recue);
}   