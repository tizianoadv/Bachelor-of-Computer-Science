/**
 * udp_gestion_requetes.c 
 * -> Fichier regroupant les fonctions utilisées pour la gestion des requetes udp.
 * Ce fichier est découpé en 2 parties :
 *  -> Partie SERVEUR 
 *  -> Partie CLIENT
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>                   /* Pour memset */

#include "udp_gestion_requetes.h"     /* Pour les prototypes */
#include "fichier.h"                  /* Pour les fonction de gestion de fichier */
#include "ncurses.h"                  /* Pour les fonction de gestion d'interface */


/**
 * Traitement des requêtes UDP encvoyées par un client et des réponses envoyées
 * @param[out] requete_recue La requete qui est recue
 * @param[out] requete_envoie La requete qu'on envoie
 * @param[out] interface L'interface ncurses
 */
void serveur_gestion_udp(requete_udp_t *requete_recue, requete_udp_t *requete_envoie,liste_sem_t *partie1, liste_sem_t *partie2,int num_port_tcp){
    int nb_parties_courantes=0;
    int nb_joueurs_partie=0;
    int num_partie_existante=0;
    int infos_correctes=0;
    
    memset(requete_envoie, 0, sizeof(requete_udp_t));
    switch(requete_recue->id){
        case LISTE_CARTE:
            requete_envoie->id = LISTE_CARTE;
            requete_envoie->liste_carte = recuperer_liste_carte();
        break;

        case LISTE_SCENARIO:
            requete_envoie->id = LISTE_SCENARIO;
            requete_envoie->liste_scenario = recuperer_liste_scenario();
        break;

        /* Démarrer partie */
        case DEMARRAGE_PARTIE:
            infos_correctes = infos_partie_correctes(&requete_recue->carte, &requete_recue->scenario);
            if( infos_correctes ){
                /* On récupère le nombre de parties */
                nb_parties_courantes = recuperer_nombre_parties(partie1, partie2);
                if( nb_parties_courantes == 0 ){
                    /* S'il n'y a aucune partie existante */
                    requete_envoie->id = DEMARRAGE_PARTIE;
                    requete_envoie->num_port_connexion_tcp = num_port_tcp;
                    requete_envoie->num_partie = 0;
                    memcpy(&requete_envoie->carte, &requete_recue->carte, sizeof(carte_t));
                    memcpy(&requete_envoie->scenario, &requete_recue->scenario, sizeof(scenario_t));
                }else if( nb_parties_courantes  <= NB_MAX_PARTIE ) {
                    /* S'il y a une seule partie existante */
                    /* On récupère le numéro de la partie existante */
                    num_partie_existante = partie_existe(partie1, partie2 , &requete_recue->carte, &requete_recue->scenario);

                    if( num_partie_existante != 0 ){
                        /* On récupère le nombre de joueurs dans la partie existante */
                        nb_joueurs_partie= recuperer_nombre_joueur_partie(partie1, partie2, num_partie_existante);
                        if( nb_joueurs_partie < 4 ){
                            /* Si le nombre de joueurs dans cette partie est inférieur à 4 */
                            requete_envoie->id = DEMARRAGE_PARTIE;
                            requete_envoie->num_port_connexion_tcp = num_port_tcp;
                            requete_envoie->num_partie = num_partie_existante;
                            memcpy(&requete_envoie->carte, &requete_recue->carte, sizeof(carte_t));
                            memcpy(&requete_envoie->scenario, &requete_recue->scenario, sizeof(scenario_t));
                        }else{      
                            /* Si le nombre de joueurs dans cette partie est supérieur ou égal à 4 */                  
                            requete_envoie->id = DEMARRAGE_PARTIE;
                            requete_envoie->num_port_connexion_tcp = num_port_tcp;
                            requete_envoie->num_partie = 0;
                            memcpy(&requete_envoie->carte, &requete_recue->carte, sizeof(carte_t));
                            memcpy(&requete_envoie->scenario, &requete_recue->scenario, sizeof(scenario_t));
                        }
                    }else if( num_partie_existante == 0 && nb_parties_courantes < 2 ) {
                        requete_envoie->id = DEMARRAGE_PARTIE;
                        requete_envoie->num_port_connexion_tcp = num_port_tcp; 
                        requete_envoie->num_partie = 0;
                        memcpy(&requete_envoie->carte, &requete_recue->carte, sizeof(carte_t));
                        memcpy(&requete_envoie->scenario, &requete_recue->scenario, sizeof(scenario_t));
                    }else{
                       requete_envoie->id = NB_MAX_PARTIE_ATTEINT; 
                    }
                }else{
                    requete_envoie->id = NB_MAX_PARTIE_ATTEINT;
                }
            }else{
                requete_envoie->id = ERREUR_REQUETE;
            }
        break;

        case LISTE_PARTIE_DEMAREES:
            requete_envoie->id = LISTE_PARTIE_DEMAREES;
            requete_envoie->liste_partie = recuperer_liste_partie(partie1, partie2);
        break;

        default:
            requete_envoie->id = ERREUR_REQUETE;
    }
}


/**
 * Menu temporaire pour le client
 * @param[out] requete_recue La requete qui est recue
 * @param[out] requete_envoie La requete qu'on envoie
 * @param[out] interface L'interface ncurses
 */
void menu(requete_udp_t *requete_recue ,requete_udp_t *requete_envoie, interface_t *interface){
    int i;
    int ch=0;
    bool continuer = FALSE;

    memset(requete_envoie, 0, sizeof(requete_udp_t));  
    wclear(interface->infos->interieur);
    wprintw(interface->infos->interieur, "\t\t* MENU *\n");
    wprintw(interface->infos->interieur, "1. Afficher la liste des cartes\n");
    wprintw(interface->infos->interieur, "2. Afficher la liste des scenarios\n");
    wprintw(interface->infos->interieur, "3. Démarrer une partie\n"); 
    wprintw(interface->infos->interieur, "4. Afficher la liste des parties démarées\n"); 
    wprintw(interface->infos->interieur, "-> Choix : "); 
    wrefresh(interface->infos->interieur);
    while(continuer == FALSE){
        ch = getch();
        if( ch >= 49 && ch <= 52 ){
            continuer = true;
            wprintw(interface->infos->interieur, "%d\n", ch-48); 
            wrefresh(interface->infos->interieur);
        } 
        else{
            wprintw(interface->infos->interieur, "\n-> Choix : %d", ch-48); 
            wrefresh(interface->infos->interieur);
        }
    }
    continuer = FALSE;
    requete_envoie->id = ch - 48;

    if( requete_envoie->id == DEMARRAGE_PARTIE){
        wclear(interface->infos->interieur);
        wprintw(interface->infos->interieur, "\t\t* LISTE PARTIES *\n");
        for(i=0 ; i < NB_MAX_PARTIE ; i++){
            if( requete_recue->liste_partie.parties[i].nom[0] == 'p' ){
                wprintw(interface->infos->interieur, "%d. %s\t",i+1,requete_recue->liste_partie.parties[i].nom);
                wprintw(interface->infos->interieur, "Carte : %s\n",requete_recue->liste_partie.parties[i].carte.nom);
                wprintw(interface->infos->interieur, "\t\tScenario : %s\n",requete_recue->liste_partie.parties[i].scenario.nom);
            }else{
                wprintw(interface->infos->interieur, "%d. ...\n",i+1);
            }   
        }
        wprintw(interface->infos->interieur, "Choix carte (1.easy, 2.hard, 3.medium) : ");
        wrefresh(interface->infos->interieur);
        while(continuer == FALSE){
            ch = getch();
            if( ch >= 49 && ch <= 51 ){
                continuer = true;
                wprintw(interface->infos->interieur, "%d\n", ch-48); 
            } 
            else{
                wprintw(interface->infos->interieur, "\nChoix carte (1.easy, 2.hard, 3.medium) : %d", ch-48); 
            }
            wrefresh(interface->infos->interieur);
        }
        continuer = FALSE;
        requete_envoie->carte.id = ch-48;
        wprintw(interface->infos->interieur, "Choix scenario (1.infini, 2.pognon, 3.tranquille) : ");
        wrefresh(interface->infos->interieur);
        while(continuer == FALSE){
            ch = getch();
            if( ch >= 49 && ch <= 51 ){
                continuer = true;
                wprintw(interface->infos->interieur, "%d\n", ch-48); 
            } 
            else{
                wprintw(interface->infos->interieur, "\nChoix scenario (1.infini, 2.pognon, 3.tranquille) : %d\n", ch-48); 
            }
            wrefresh(interface->infos->interieur);
        }
        continuer = FALSE;
        requete_envoie->scenario.id = ch-48;         
        requete_envoie->id = DEMARRAGE_PARTIE;

        switch(requete_envoie->carte.id){
            case 1:
                strcpy(requete_envoie->carte.nom,"easy");
            break;
            case 2:
                strcpy(requete_envoie->carte.nom,"hard");
            break;
            case 3:
                strcpy(requete_envoie->carte.nom,"medium");
            break;
        }
        switch(requete_envoie->scenario.id){
            case 1:
                strcpy(requete_envoie->scenario.nom,"infini");
            break;
            case 2:
                strcpy(requete_envoie->scenario.nom,"pognon");
            break;
            case 3:
                strcpy(requete_envoie->scenario.nom,"tranquille");
            break;
        }
    }
    memset(requete_recue, 0, sizeof(requete_udp_t));
}

/**
 * Traitement des requêtes UDP encvoyées par le serveur et des réponses envoyées
 * @param[out] requete_recue La requete qui est recue
 * @param[out] requete_envoie La requete qu'on envoie
 * @param[out] interface L'interface ncurses
 */
void client_gestion_udp(requete_udp_t *requete_recue, requete_udp_t *requete_envoie, interface_t *interface){
    int i;
    int ch;
    char chaine_vide[256]={0};
    bool continuer = FALSE;

    memset(requete_envoie, 0, sizeof(requete_udp_t));
    wclear(interface->infos->interieur);
    switch(requete_recue->id){
        case LISTE_CARTE:
            wprintw(interface->infos->interieur, "\t\t* LISTE CARTES *\n");
            for(i=0 ; i < NB_MAX_CARTE ; i++){
                if( strcmp(requete_recue->liste_carte.cartes[i].nom, chaine_vide) != 0 )
                    wprintw(interface->infos->interieur, "%d. %s\n", i+1, requete_recue->liste_carte.cartes[i].nom);
                else
                    wprintw(interface->infos->interieur, "%d. ...\n", i+1);
            }
            wprintw(interface->infos->interieur, "\n-> Appuyer sur 'c' pour continuer ...\n");
            wrefresh(interface->infos->interieur);
        break;

        case LISTE_SCENARIO:
            wprintw(interface->infos->interieur, "\t\t* LISTE SCENARIOS *\n");
            for(i=0 ; i < NB_MAX_SCENARIO ; i++){
                if( strcmp(requete_recue->liste_scenario.scenarios[i].nom, chaine_vide) != 0  )
                    wprintw(interface->infos->interieur, "%d. %s\n", i+1, requete_recue->liste_scenario.scenarios[i].nom);
                else
                    wprintw(interface->infos->interieur, "%d. ...\n", i+1);
            }
            wprintw(interface->infos->interieur, "\n-> Appuyer sur 'c' pour continuer ...\n");
            wrefresh(interface->infos->interieur);
        break;

        case DEMARRAGE_PARTIE:
            wprintw(interface->infos->interieur, "\t\t* DEMARRAGE PARTIE *\n");
            wprintw(interface->infos->interieur, "-> Numéro port connexion tcp : %d\n",requete_recue->num_port_connexion_tcp);
            wprintw(interface->infos->interieur, "-> Nom carte : %s\n",requete_recue->carte.nom);
            wrefresh(interface->infos->interieur); 
        break;

        case LISTE_PARTIE_DEMAREES:
            wprintw(interface->infos->interieur, "\t\t* LISTE PARTIES DEMAREES *\n");
            for(i=0 ; i < NB_MAX_PARTIE ; i++){

                if( strcmp(requete_recue->liste_partie.parties[i].nom,"partie1") == 0 || strcmp(requete_recue->liste_partie.parties[i].nom,"partie2") == 0 ){
                    wprintw(interface->infos->interieur, "%d. %s\t",i+1,requete_recue->liste_partie.parties[i].nom);
                    wprintw(interface->infos->interieur, "Carte : %s\n",requete_recue->liste_partie.parties[i].carte.nom);
                    wprintw(interface->infos->interieur, "\t\tScenario : %s\n",requete_recue->liste_partie.parties[i].scenario.nom);
                }
                else
                    wprintw(interface->infos->interieur, "%d. ...\n",i+1);
            }
            wprintw(interface->infos->interieur, "\n-> Appuyer sur 'c' pour continuer ...\n");
            wrefresh(interface->infos->interieur); 
        break;

        case NB_MAX_PARTIE_ATTEINT:
            wprintw(interface->infos->interieur, "\n\t\t* NOMBRE MAX PARTIE ATTEINT *\n");
            wprintw(interface->infos->interieur, "\n-> Appuyer sur 'c' pour continuer ...\n");
            wrefresh(interface->infos->interieur); 
        break;

        default:
            wprintw(interface->infos->interieur, "\n\t\t* ERREUR REQUETES *\n");
            wprintw(interface->infos->interieur, "\n-> Appuyer sur 'c' pour continuer ...\n");
            wrefresh(interface->infos->interieur); 
    }

    if( requete_recue->id != DEMARRAGE_PARTIE ){
        while(continuer == FALSE){
            ch = getch();
            if((ch == 'C') || (ch == 'c'))
                continuer = true;
            else{
                wprintw(interface->infos->interieur, "-> Appuyer sur 'c' pour continuer ...\n");
                wrefresh(interface->infos->interieur);
            }
        }
        continuer = FALSE;

        wclear(interface->infos->interieur);
        menu(requete_recue ,requete_envoie, interface);
    }
        
}