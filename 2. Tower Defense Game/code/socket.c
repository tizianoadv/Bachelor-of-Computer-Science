/**
 * socket.c 
 * -> Gestion de création de socket UDP + TCP et d'adresse réseau compréhensible par le système
 * -> Reception et envoie de requêtes UDP + TCP
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>         /* Pour memset */
#include <sys/socket.h>     /* Pour bind */
#include <errno.h>          /* Pour errno */
#include <unistd.h>         /* Pour close */ 

#include "socket.h"         /* Pour les prototypes */

/*** PARTIE TCP & UDP ***/

/**
 * Création de d'une adresse serveur réseau aléatoire
 * @param[out] socket (socket_t *)
 * @param[out] num_port_serveur (in_port_t)
 **/
void creer_adresseServeur_reseau_aleatoire(socket_t *socket, in_port_t num_port_serveur){

    memset(&socket->adresseServeur, 0, sizeof(struct sockaddr_in));
    socket->adresseServeur.sin_family = AF_INET;
    socket->adresseServeur.sin_port = htons( num_port_serveur );
    socket->adresseServeur.sin_addr.s_addr = htonl(INADDR_ANY);
}
/**
 * Création de l'adresse serveur réseaux
 * @param[out] socket (socket_t *)
 * @param[out] adresseServeur_p (char*)
 * @param[out] num_port_serveur (in_port_t)
 **/
void creer_adresseServeur_reseau(socket_t *socket, char *adresseServeur_p, in_port_t num_port_serveur){

    memset(&socket->adresseServeur, 0, sizeof(struct sockaddr_in));
    socket->adresseServeur.sin_family = AF_INET;
    socket->adresseServeur.sin_port = htons( num_port_serveur );
    if(inet_pton(AF_INET, adresseServeur_p, &socket->adresseServeur.sin_addr) != 1) {
        perror("Erreur lors de la conversion de l'adresse ");
        exit(EXIT_FAILURE);
    }
}
/**
 * Nommage de la socket serveur
 * @param[out] socket (socket_t *)
 **/
void nommer_socket_serveur(socket_t *socket){

    if(bind(socket->fd, (struct sockaddr*) &socket->adresseServeur , sizeof(struct sockaddr_in)) == -1) {
        perror("Erreur lors du nommage de la socket ");
        exit(EXIT_FAILURE);
    }
}
/**
 * Fermeture des socket
 * @param[out] socket (socket_t *)
 **/
void fermer_socket(socket_t *socket){
    if(close(socket->fd) == -1) {
        perror("Erreur lors de la fermeture de la socket ");
        exit(EXIT_FAILURE);
    }
}

/*** PARTIE UDP ***/
/**
 * Création de la socket UDP
 **/
socket_t creer_socket_udp(){
    socket_t retour;

    /*Initialisation partie serveur */
    memset(&retour,0,sizeof(socket_t));
    /*Création de la socket */
    if((retour.fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Erreur lors de la création de la socket ");
        exit(EXIT_FAILURE);
    }
    return retour;
}

/* PARTIE SERVEUR - UDP  */
/**
 * Création de l'adresse serveur réseaux
 * @param[out] socket (socket_t *)
 * @param[out] recu (requete_udp_t *)
 **/
void lecture_requete_udp(socket_t *socket, requete_udp_t *recu){
    char chaine_adresseClient[INET_ADDRSTRLEN]={0};

    /* Initialisation des variables avant reception de message */
    socket->taille_adresseClient = sizeof(struct sockaddr);
    memset(&socket->adresseClient, 0, sizeof(struct sockaddr));
    memset(recu, 0, sizeof(requete_udp_t));

    printf("Serveur UDP : en attente du message du client.\n");
    if(recvfrom(socket->fd, recu, sizeof(requete_udp_t), 0, &socket->adresseClient, &socket->taille_adresseClient) == -1) {
        perror("Erreur lors de la réception du message ");
        exit(EXIT_FAILURE);
    }

    /* Convertion d'une adresse réseau client recue */
    if( (inet_ntop(socket->adresseClient.sa_family, &socket->adresseClient, chaine_adresseClient, INET_ADDRSTRLEN)) == NULL ){
        perror("Erreur conversion adresse client vers chaine");
        exit(EXIT_FAILURE);
    }
    printf("Adresse client : %s\n", chaine_adresseClient);
}

/**
 * Envoie d'une réponse udp
 * @param[out] socket (socket_t *)
 * @param[out] requete (requete_udp_t *)
 **/
void envoyer_reponse_udp(socket_t *socket, requete_udp_t *requete){

    /* Envoi du message */
    if(sendto(socket->fd, requete, sizeof(requete_udp_t), 0, (struct sockaddr*) &socket->adresseClient, sizeof(struct sockaddr_in)) == -1) {
        if( errno == EACCES )
            printf("EACCES\n");
        else if (errno == EAGAIN)
            printf("EAGAIN\n");
        else 
           perror("Erreur lors de l'envoi du message ");
        exit(EXIT_FAILURE);
    }
    printf("Serveur UDP : message envoyé.\n");
}

/* PARTIE CLIENT - UDP */
/**
 * Lecture d'une réponse par le serveur
 * @param[out] socket (socket_t *)
 * @param[out] recu (requete_udp_t *)
 **/
void lecture_reponse_udp(socket_t *socket, requete_udp_t *recu){
    memset(recu, 0, sizeof(requete_udp_t));
    if(recvfrom(socket->fd, recu, sizeof(requete_udp_t), 0, NULL, NULL) == -1) {
        perror("Erreur lors de la réception du message ");
        exit(EXIT_FAILURE);
    }
}

/**
 * Envoie d'une requete udp
 * @param[out] socket (socket_t *)
 * @param[out] requete (requete_udp_t *)
 **/
void envoyer_requete_udp(socket_t *socket, requete_udp_t *requete){
    /* Envoi du message */
    if(sendto(socket->fd, requete, sizeof(requete_udp_t), 0, (struct sockaddr*) &socket->adresseServeur, sizeof(struct sockaddr_in)) == -1) {
        if( errno == EACCES )
            printf("EACCES\n");
        else if (errno == EAGAIN)
            printf("EAGAIN\n");
        else 
           perror("Erreur lors de l'envoi du message ");

        exit(EXIT_FAILURE);
    }
}


/*** PARTIE TCP ***/
/**
 * Création de la socket TCP 
 **/
socket_t creer_socket_tcp(){
    socket_t retour;
    memset(&retour, 0, sizeof(socket_t));
    if((retour.fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("Erreur lors de la création de la socket ");
        exit(EXIT_FAILURE);
    }
    return retour;
}

/**
 * Reception de requete TCP
 * @param[out] socket_communication (socket_t *)
 * @param[out] requete (requete_tcp_t *)
 **/
void receptionner_requete_tcp(socket_t *socket_communication, requete_tcp_t *requete){
    memset(requete, 0, sizeof(requete_tcp_t));
    if(read(socket_communication->fd, requete, sizeof(requete_tcp_t)) == -1) {
        perror("Erreur lors de la lecture de la taille du message ");
        exit(EXIT_FAILURE);
    }
}

/**
 * Envoie de requete TCP
 * @param[out] socket_communication (socket_t *)
 * @param[out] reponse (requete_tcp_t *)
 **/
void envoyer_requete_tcp(socket_t *socket_communication, requete_tcp_t *reponse){

    if(write(socket_communication->fd, reponse, sizeof(requete_tcp_t)) == -1) {
        perror("Erreur lors de l'envoi du message ");
        exit(EXIT_FAILURE);
    }
    memset(reponse, 0, sizeof(requete_tcp_t));
}

/* PARTIE SERVEUR - TCP */
/**
 * Mise de la socket en mode passive (écoute)
 * @param[out] socket (socket_t *)
 **/
void mettre_socket_passive(socket_t *socket){
    if(listen(socket->fd, NB_MAX_CLIENTS_ATTENTE) == -1) {
        perror("Erreur lors de la mise en mode passif ");
        exit(EXIT_FAILURE);
    }
}

/**
 * Ecoute les connections de client 
 * @param[out] socket_connexion (socket_t *)
 **/
socket_t ecouter_connexion(socket_t *socket_connexion){
    socket_t socket_communication;
        
    /* Initialisation des variables avant reception de message */
    socket_connexion->taille_adresseClient = sizeof(struct sockaddr);
    memset(&socket_connexion->adresseClient, 0, sizeof(struct sockaddr));
    memset(&socket_connexion->taille_adresseClient, 0, sizeof(socklen_t));
    memset(&socket_communication, 0, sizeof(socket_t));
    memcpy(&socket_communication.adresseServeur, &socket_connexion->adresseServeur, sizeof(struct sockaddr_in));
    
    printf("FILS : attente de connexion...\n");
    if((socket_communication.fd = accept(socket_connexion->fd, &socket_connexion->adresseClient, &socket_connexion->taille_adresseClient)) == -1) {
        perror("Erreur lors de la demande de connexion ");
        exit(EXIT_FAILURE);
    }

    /* Récupération de l'adresse du client pour socket communication */
    memcpy(&socket_communication.adresseClient, &socket_connexion->adresseClient, sizeof(struct sockaddr));
    memcpy(&socket_communication.taille_adresseClient, &socket_connexion->taille_adresseClient, sizeof(socklen_t));
    return socket_communication;
}

/* PARTIE CLIENT - TCP */
/**
 * Connexion du client au serveur
 * @param[out] socket (socket_t *)
 **/
void connecter_serveur(socket_t *socket){

    if((connect(socket->fd, (struct sockaddr*)&socket->adresseServeur, sizeof(struct sockaddr_in))) == -1) {
        perror("Erreur lors de la connexion ");
        exit(EXIT_FAILURE);
    }
}

