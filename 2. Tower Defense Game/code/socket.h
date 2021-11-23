#ifndef _SOCKET_
#define _SOCKET_

/* INCLUDES */
#include <arpa/inet.h>              /* Pour la structure struct sockaddr_in */
#include "fichier.h"                /* Pour les types carte_t, scenario_t, liste_carte_t, liste_scenario_t */
#include "udp_gestion_sem.h"        /* Pour les types partie_t et liste_partie_t */
 
/* COSNTANTES */
#define NB_MAX_CLIENTS_ATTENTE 8

/* STRUCTURES */
/* Structures d'une socket */
typedef struct{
    int fd;
    struct sockaddr_in adresseServeur;
    struct sockaddr adresseClient;
    socklen_t taille_adresseClient; 
}socket_t;

/* Structures d'une requete UDP */
typedef struct{
    int id;
    carte_t carte;
    scenario_t scenario;
    liste_carte_t liste_carte;
    liste_scenario_t liste_scenario;
    liste_partie_t liste_partie;
    in_port_t num_port_connexion_tcp;
    int num_partie;
}requete_udp_t;

/* Structures d'une requete TCP pour le jeu*/
typedef struct{
    int id;
    int id_partie;
    int id_joueur;
    int id_adv;
    int etat_connexion;
    carte_t carte;
    scenario_t scenario;
    jeu_t jeu;
    ligne_scenario_t ligne_scenario;
}requete_tcp_t;


/* PROTOTYPES */
/*** PARTIE TCP & UDP ***/
/**
 * Création de d'une adresse serveur réseau aléatoire
 * @param[out] socket (char*)
 * @param[out] num_port_serveur (in_port_t)
 **/
void creer_adresseServeur_reseau_aleatoire(socket_t *socket, in_port_t num_port_serveur);

/**
 * Création de l'adresse serveur réseaux
 * @param[out] socket (char*)
 * @param[out] adresseServeur_p (char*)
 * @param[out] num_port_serveur (in_port_t)
 **/
void creer_adresseServeur_reseau(socket_t *socket, char *adresseServeur_p, in_port_t num_port_serveur);

/**
 * Nommage de la socket serveur
 * @param[out] socket (socket_t *)
 **/
void nommer_socket_serveur(socket_t *socket);

/**
 * Fermeture des socket
 * @param[out] socket (socket_t *)
 **/
void fermer_socket(socket_t *socket);


/*** PARTIE UDP ***/
/**
 * Création de la socket UDP
 **/
socket_t creer_socket_udp();

/* PARTIE SERVEUR - UDP  */
/**
 * Création de l'adresse serveur réseaux
 * @param[out] socket (socket_t *)
 * @param[out] recu (requete_udp_t *)
 **/
void lecture_requete_udp(socket_t *socket, requete_udp_t *recu);

/**
 * Envoie d'une réponse udp
 * @param[out] socket (socket_t *)
 * @param[out] requete (requete_udp_t *)
 **/
void envoyer_reponse_udp(socket_t *socket, requete_udp_t *requete);

/* PARTIE CLIENT - UDP */
/**
 * Lecture d'une réponse par le serveur
 * @param[out] socket (socket_t *)
 * @param[out] recu (requete_udp_t *)
 **/
void lecture_reponse_udp(socket_t *socket, requete_udp_t *recu);

/**
 * Envoie d'une requete udp
 * @param[out] socket (socket_t *)
 * @param[out] requete (requete_udp_t *)
 **/
void envoyer_requete_udp(socket_t *socket, requete_udp_t *requete);


/*** PARTIE TCP ***/
/**
 * Création de la socket TCP 
 **/
socket_t creer_socket_tcp();

/**
 * Reception de requete TCP
 * @param[out] socket_communication (socket_t *)
 * @param[out] requete (requete_tcp_t *)
 **/
void receptionner_requete_tcp(socket_t *socket_communication, requete_tcp_t *requete);

/**
 * Envoie de requete TCP
 * @param[out] socket_communication (socket_t *)
 * @param[out] reponse (requete_tcp_t *)
 **/
void envoyer_requete_tcp(socket_t *socket_communication, requete_tcp_t *reponse);

/* PARTIE SERVEUR - TCP */
/**
 * Mise de la socket en mode passive (écoute)
 * @param[out] socket (socket_t *)
 **/
void mettre_socket_passive(socket_t *socket);

/**
 * Ecoute les connections de client 
 * @param[out] socket_connexion (socket_t *)
 **/
socket_t ecouter_connexion(socket_t *socket_connexion);

/* PARTIE CLIENT - TCP */
/**
 * Connexion du client au serveur
 * @param[out] socket (socket_t *)
 **/
void connecter_serveur(socket_t *socket);

#endif
