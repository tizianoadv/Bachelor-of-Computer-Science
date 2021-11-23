/**
 * tcp_connexion.c 
 * -> Fichier regroupant les fonctions utilisées pour la gestion des connexions tcp.
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */

/* MACRO POUR COMPILATION -ANSI AVEC SIGACTION ET AVEC SA_RESTART */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>          /* Pour fork */
#include <signal.h>          /* Pour sigaction */
#include <sys/types.h>       /* Pour les types */
#include <sys/wait.h>        /* Pour waitpid */

#include "socket.h"           /* Pour les fonctions gestion de socket TCP */
#include "tcp_connexion.h"    /* Pour les prototypes */
#include "tcp_communication.h"/* Pour la fonction de communication avec le client */

/* VARIABLES GLOBALES */
int stop_fils=0;
pid_t pid_petits_fils[NB_MAX_CONNEXION] = {0};

/**
 * Gestionnaire pour SIGCHLD des petits fils
 * @param[out] signum  Le numéro du signal
 */
void gestionnaire_p_f(int signum){
    int i=0;
    int pid_p_f=0;
    int pid_ok = 0;
    int statut=0;

    if((pid_p_f = wait(&statut)) == -1 )
        perror("Erreur lors de l'attente du fils ");
    if(WIFEXITED(statut)){
        while( (i < NB_MAX_CONNEXION) && (pid_ok == 0) ){
            if( (pid_p_f == pid_petits_fils[i]) ){
                pid_ok = 1;
                stop_fils++;
                printf("FILS : petit fils (pid:%d) à terminé \n",pid_p_f);
                printf("FILS :  stop_fils = %d\n",stop_fils);
            } 
            i++;
        }  
    }
    else
        printf("Le petit fils à terminé anormalement");

    pid_ok = 0;
    statut=0;
}

/**
 * Demarrage du fils qui gère les connexion TCP
 * @param[out] pid_fils  Le pid du fils
 * @param[out] port_tcp  Le numéro du port tcp
 * @param[out] sem       Le segment de mémoire
 * @param[out] partie1   La première partie
 * @param[out] partie2   La seconde partie
 */
void creer_fils(int *pid_fils, int port_tcp, sem_t *sem, liste_sem_t *partie1, liste_sem_t *partie2){
    if( (*pid_fils = fork()) == -1 ){
        perror("Erreur lors de création du fils ");
        exit(EXIT_FAILURE);
    }
    if( *pid_fils == 0){
        /* On est dans le fils */
        demarrer_socket_connexion(port_tcp, sem, partie1, partie2);
        exit(EXIT_SUCCESS);
    }
}

/**
 * Demarrage le processus d'attente de connexion TCP
 * @param[out] port_tcp  Le numéro du port tcp
 * @param[out] sem       Le segment de mémoire
 * @param[out] partie1   La première partie
 * @param[out] partie2   La seconde partie
 */
void demarrer_socket_connexion(int port_tcp, sem_t *sem, liste_sem_t *partie1, liste_sem_t *partie2){
    int nb_connexion=0;
    socket_t socket_connexion;
    socket_t socket_communication;
    struct sigaction action_p_f;

    /* PARTIE POSTIONEMENT SIGNAL SIGCHLD DES PETITS FILS */
    sigemptyset(&action_p_f.sa_mask);
    action_p_f.sa_flags = SA_RESTART;
    action_p_f.sa_handler = gestionnaire_p_f;
    sigaction(SIGCHLD, &action_p_f, NULL);

    printf("FILS : Créé ! (pid : %d)\n",getpid());
    /* PARTIE CONNEXION TCP */
    /* Création de la socket tcp */
    socket_connexion = creer_socket_tcp();
    /* Création de l'adresse serverur réseaux de la socket tcp*/
    creer_adresseServeur_reseau_aleatoire(&socket_connexion, port_tcp );
    /* Nommage de la socket udp serveur */
    nommer_socket_serveur(&socket_connexion);
    /* Mise de la socket en mode passive (écoute) */
    mettre_socket_passive(&socket_connexion);

    /* PARTIE CREATION DES PETITS FILS */
    while( nb_connexion < NB_MAX_CONNEXION ){

        /* Ecoute les connections de client */
        socket_communication = ecouter_connexion(&socket_connexion);

        /* Création des petits fils qui gère les communications TCP */
        creer_petits_fils(&socket_connexion, &socket_communication, sem, nb_connexion, partie1, partie2);

        nb_connexion++;
    }

    /* PARTIE ATTENTE FIN DES CLIENTS */
    while(stop_fils < NB_MAX_CONNEXION ){
        printf("FILS : En attente fin des petits fils !\n");
        sleep(5);
    }
    
    detacher_sem(sem);
    fermer_socket(&socket_connexion);
    printf("FILS : Terminé !\n");
}

/**
 * Création des petits fils qui gère les communications TCP
 * @param[out] socket_connexion  La socket de connexion
 * @param[out] socket_communication  La socket de communication
 * @param[out] sem        Le segment de mémoire
 * @param[out] num_petit_fils Le numéro du petit fils
 * @param[out] partie1   La première partie
 * @param[out] partie2   La seconde partie
 */
void creer_petits_fils(socket_t *socket_connexion, socket_t *socket_communication, sem_t *sem, int num_petit_fils, liste_sem_t *partie1, liste_sem_t *partie2){

    if((pid_petits_fils[num_petit_fils] = fork()) == -1 ){
        perror("Erreur lors de création du petit fils");
        exit(EXIT_FAILURE);
    }
    if(pid_petits_fils[num_petit_fils] == 0 ){
        fermer_socket(socket_connexion);
        printf("PETIT FILS %d : Créé ! (pid : %d)\n",num_petit_fils, getpid());
        
        /* Communication avec un client */
        communiquer_avec_client(socket_communication, sem, partie1, partie2);
        
        fermer_socket(socket_communication);
        sleep(1);
        exit(EXIT_SUCCESS);
    }
    if(pid_petits_fils[num_petit_fils] > 0){
        fermer_socket(socket_communication);
    }
}