/* MACRO POUR COMPILATION -ANSI AVEC SIGACTION ET AVEC SA_RESTART */
#define _POSIX_C_SOURCE 200809L

/**
 * serveur.c 
 * -> Gestion du segment de mémoire partagée
 *      -> Création + attachement
 * -> Intialisation des listes qui pointes sur le segment
 * -> Création et positionnement de socket UDP
 * -> Création du numéro de port aléatoire de la socket de connexion
 * -> Création du fils qui gère les connexions TCP
 * -> Attente de fin du fils
 * -> Lecture & traitement & envoie requêtes UDP reçues par et à destination d'un client
 * -> Fermeture de toutes les declarations
 * -> Arrêt du serveur CTRL + C (SIGINT)
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>                 /* Pour memset et strcpy */
#include <time.h>                   /* Pour srand et rand */
#include <sys/wait.h>               /* Pour waitpid */
#include <signal.h>                 /* Pour sigaction */
#include <unistd.h>                 /* Pour sleep */

#include "udp_gestion_sem.h"        /* Pour fonctions gestions de liste et sem*/
#include "tcp_connexion.h"          /* Pour demarrage de la socket de connexion */
#include "socket.h"                 /* Pour fonctions sockets */
#include "udp_gestion_requetes.h"   /* Pour fonctions gestions requêtes UDP */
#include "fichier.h"                /* Pour fonctions gestions de fichier */
#include "sem.h"                    /* Pour fonctions gestions de segment de mémoire */
#include "liste.h"                  /* Pour fonctions gestions de liste */

/* VARIABLES GLOBALES */
pid_t pid_fils=0;
int stop_serveur=0;

/* Gestionnaire pour SIGCHLD du fils */
void gestionnaire_fils(int signum){
    int statut;
    if(waitpid(pid_fils ,&statut, 0) == -1 )
        perror("Erreur lors de l'attente du fils ");
    if(WIFEXITED(statut)){
        printf("SERVEUR UDP : fils (pid:%d) à terminé \n",pid_fils);
        stop_serveur++;
    }
    else
        printf("Le fils à terminé anormalement");
}

int main(int argc, char* argv[]){
    int port_tcp_min = 12500;
    int port_tcp_max = 13000;
    int num_port_tcp = 0;
    socket_t socket_udp;
    requete_udp_t requete_recue;
    requete_udp_t requete_envoie;
    struct sigaction action_fils;
    sem_t sem;
    liste_sem_t partie1;
    liste_sem_t partie2;

    /* PARTIE ARGUMENT */
    /* Vérification des arguments */
    if(argc != 2) {
        fprintf(stderr, "Usage : %s port\n", argv[0]);
        fprintf(stderr, "Où :\n");
        fprintf(stderr, "  port : le numéro de port d'écoute du serveur\n");
        exit(EXIT_FAILURE);
    }

    /* PARTIE SEGMENT MEMOIRE */
    /* Création du segment de mémoire */
    sem = creer_sem();
    /* Attachement au segment de mémoire */
    attacher_sem(&sem);
    
    /* PARTIE LISTE */
    /*Création des listes partie 1 & 2*/
    partie1 = creer_liste();
    partie2 = creer_liste();    
    /*Attacher les listes au segment*/
    attacher_liste_sem(&partie1,&sem,PARTIE_1);
    attacher_liste_sem(&partie2,&sem,PARTIE_2);
    /*Initialisation des parties du segment */
    initialiser_parties(&partie1);
    initialiser_parties(&partie2);
    
    /* PARTIE CREATION FILS - POUR CONNEXION TCP */
    srand(time(NULL));
    num_port_tcp = (rand() % (port_tcp_max - port_tcp_min + 1)) + port_tcp_min;
    creer_fils(&pid_fils, num_port_tcp, &sem, &partie1, &partie2);

    /* PARTIE POSTIONEMENT SIGNAL SIGCHLD DU FILS */
    sigemptyset(&action_fils.sa_mask);
    action_fils.sa_flags = SA_RESTART;
    action_fils.sa_handler = gestionnaire_fils;
    sigaction(SIGCHLD, &action_fils, NULL);

    /* PARTIE SOCKET - UDP */
    /* Création de la socket udp */
    socket_udp = creer_socket_udp();
    /* Création de l'adresse serverur réseaux de la socket udp */
    creer_adresseServeur_reseau_aleatoire(&socket_udp, atoi(argv[1]));
    /* Nommage de la socket udp serveur */
    nommer_socket_serveur(&socket_udp);

    /* PARTIE SERVEUR UDP */
    if( pid_fils > 0 ){

        while(1){

            /* Lecture d'un message de la part d'un client */
            lecture_requete_udp(&socket_udp, &requete_recue);
            printf("SERVEUR UDP recu : %d\n", requete_recue.id);

            /* Traitement requête udp */
            serveur_gestion_udp(&requete_recue, &requete_envoie, &partie1, &partie2, num_port_tcp);

            /* Envoie d'une réponse udp */
            envoyer_reponse_udp(&socket_udp, &requete_envoie);
        }

        /* Fermeture des socket */
        fermer_socket(&socket_udp);
        /*Détachement du segement de mémoire*/
        detacher_sem(&sem);
        /*Suppression du segment de mémoire*/
        supprimer_sem(&sem);
        /*Suppression des listes*/
        supprimer_liste(&partie1);
        supprimer_liste(&partie2);
    }
    return EXIT_SUCCESS;
}