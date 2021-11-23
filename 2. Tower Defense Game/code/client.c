/**
 * client.c 
 * -> Affichage du menu demarrage
 * -> Discussion avec le serveur en UDP 
 *      -> Demande de liste de carte
 *      -> Demande de liste de scénario
 *      -> Demande de liste de partie démarrées
 *      -> Demande de démarrage de partie
 * -> Discussion avec le fils pour la connexion à une partie
 * -> Discussion avec le petit fils pour l'affichage du jeu personnalisé
 *      -> Affichage du jeu en temps réel 
 *      -> Affichage d'information
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>                 /* Pour strcpy et strcat */
#include <unistd.h>                 /* Pour sleep */
#include "ncurses.h"                /* Pour les fonction ncurses */

#include "socket.h"                 /* Pour fonctions sockets */
#include "udp_gestion_requetes.h"   /* Pour fonctions de gestion requêtes UDP */
#include "tcp_connexion.h"          /* Pour fonctions de gestion requete TCP de connexion */
#include "tcp_communication.h"      /* Pour fonctions de gestion requete TCP de communication */
#include "interaction.h"            /* Pour fonctions de gestion de l'interaction */

int main(int argc, char* argv[]){
    int client_stop=0;
    int CASES_ORDI[2]={0};
    socket_t socket_udp;
    socket_t socket_tcp;
    requete_udp_t requete_recue;
    requete_udp_t requete_envoie;
    requete_tcp_t requete_tcp_recu;
    interface_t interface;

    /* PARTIE ARGUMENT */
    /* Vérification des arguments */
    if(argc < 3) {
        fprintf(stderr, "Usage : %s adresseServeur portServeur\n", argv[0]);
        fprintf(stderr, "Où :\n");
        fprintf(stderr, "  adresseServeur : l'adresse d'écoute du serveur\n");
        fprintf(stderr, "  portServeur : le numéro de port d'écoute du serveur\n");
        exit(EXIT_FAILURE);
    }

    /* Initialisation de ncurses */
    ncurses_initialiser();
    ncurses_souris();
    ncurses_couleurs(); 
    palette();
    clear();
    refresh();  

    /* Vérification des dimensions du terminal */
    if((COLS < LARGEUR) || (LINES < HAUTEUR)) {
        ncurses_stopper();
        fprintf(stderr, 
                "Les dimensions du terminal sont insufisantes : l=%d,h=%d au lieu de l=%d,h=%d\n", 
                COLS, LINES, LARGEUR, HAUTEUR);
        exit(EXIT_FAILURE);
    }  

    /* PARTIE INTERFACE NCURSES */
    interface = creer_interface_demarrage();

    /* PARTIE SOCKET - UDP */
    /* Création de la socket udp */
    socket_udp = creer_socket_udp();
    /* Création de l'adresse réseaux de la socket udp */
    creer_adresseServeur_reseau(&socket_udp, argv[1], atoi(argv[2]));

    /* Envoie d'une requete udp */
    menu(&requete_recue, &requete_envoie, &interface);
    envoyer_requete_udp(&socket_udp, &requete_envoie);

    /* TANT QUE LE SERVEUR N'ENVOIE PAS DEMARRAGE_PARTIE */
    while( requete_recue.id != DEMARRAGE_PARTIE ){
        
        /* Lecture d'une réponse par le serveur */
        lecture_reponse_udp(&socket_udp, &requete_recue);

        /* Traitement gestion des requêtes UDP */
        client_gestion_udp(&requete_recue, &requete_envoie, &interface);
        /* Envoie d'une requete udp */
        if( requete_recue.id != DEMARRAGE_PARTIE )
            envoyer_requete_udp(&socket_udp, &requete_envoie);          
    }

    /* PARTIE SOCKET - TCP */
    /* Création de la socket tcp */
    socket_tcp = creer_socket_tcp();
    /*Création de l'adresse serverur réseaux de la socket tcp*/
    creer_adresseServeur_reseau(&socket_tcp, argv[1] ,requete_recue.num_port_connexion_tcp);
    /* Connexion du client au serveur */
    connecter_serveur(&socket_tcp);

    /* Communication avec un serveur*/
    connecter_partie(&socket_tcp, &interface, &requete_recue, &requete_tcp_recu,CASES_ORDI);
    
    /* AFFICHAGE ET MISE A JOUR DU JEU */
    do{
        receptionner_info_jeu(&socket_tcp, &requete_tcp_recu);
        if(requete_tcp_recu.id == UPDATE_JEU )
            affichage(&interface, &requete_tcp_recu, CASES_ORDI);
    }while(requete_tcp_recu.id == UPDATE_JEU );

    /* AFFICHAGE ET MISE A JOUR FIN JOUEUR */
    while(requete_tcp_recu.id == FIN_JOUEUR){
        if(requete_tcp_recu.id == FIN_JOUEUR )
            affichage(&interface, &requete_tcp_recu,CASES_ORDI);
        receptionner_info_jeu(&socket_tcp, &requete_tcp_recu);
    }

    /* AFFICHAGE ET MISE A JOUR FIN PARTIE*/
   do{
        if(requete_tcp_recu.id == FIN_PARTIE ){
            client_stop++;
            affichage(&interface, &requete_tcp_recu,CASES_ORDI);
        }
        receptionner_info_jeu(&socket_tcp, &requete_tcp_recu);
   }while(requete_tcp_recu.id != FIN_PARTIE && client_stop == 0);
    sleep(5);

    /* Fermeture des socket */
    fermer_socket(&socket_tcp);
    /* Fermeture des socket */
    fermer_socket(&socket_udp);
    /* Arrêt de ncurses */
    ncurses_stopper();
    return EXIT_SUCCESS;
}