/**
 * interaction.c 
 * Fichier repertoriant les fonctions de gestion de l'affichage dynamique des jeux personalisées et des informations de  la partie
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */

#include <stdio.h>
#include <stdlib.h>

#include "interaction.h"    /* Pour les prototypes*/

/**
 * Gestion des affichages
 * @param[out] interface (interface_t *)
 * @param[out] requete_recue (requete_tcp_t *)
 * @param[out] CASES_ORDI (int *)
 **/
void affichage(interface_t *interface, requete_tcp_t *requete_recue, int *CASES_ORDI){

    if( requete_recue->id == UPDATE_JEU ){
        switch(requete_recue->ligne_scenario.type){
            case 1 :
                wclear(interface->carte->interieur);
                affichage_carte(interface, &requete_recue->jeu, CASES_ORDI);
                affichage_etat(interface, &requete_recue->jeu);
                break;

            case 2 :
                /*freeze*/
                break;
            case 3 :
                /*argent*/
                break;
            default :
                break;
        }
    }
    else if(requete_recue->id == FIN_JOUEUR){
        wclear(interface->infos->interieur);
        wprintw(interface->infos->interieur,"\t\t* GAME OVER *\n");
        wprintw(interface->infos->interieur,"Attente de la fin des autres joueurs ...\n");
        wrefresh(interface->infos->interieur);
        affichage_etat(interface, &requete_recue->jeu);
    }else{
        wclear(interface->infos->interieur);
        wprintw(interface->infos->interieur,"\t\t* FIN DE LA PARTIE *\n");
        wprintw(interface->infos->interieur,"Encore une victoire écrasante de Cyril\n");
        wprintw(interface->infos->interieur,"Avec ce magnifique coup spécial le Rab bottom\n");
        wrefresh(interface->infos->interieur);
    }
}

/**
 * Affichage de la carte
 * @param[out] interface (interface_t *)
 * @param[out] jeu (jeu_t *)
 * @param[out] CASES_ORDI (int *)
 **/
void affichage_carte(interface_t *interface, jeu_t *jeu, int *CASES_ORDI){
    int i, j;
    
    for(i = 0; i < 15; i++) {
        for(j = 0; j < 15; j++) {
            if( (i == CASES_ORDI[0]) && (j == CASES_ORDI[1]) && (jeu->carte[i][j] < CASE_MAX_CHEMIN) ){
                jeu->carte[i][j] = CASE_ORDI;
            }
            if(jeu->carte[i][j] == CASE_SOLDAT) {
                wattron(interface->carte->interieur, COLOR_PAIR(COULEUR_UNITE));
                mvwprintw(interface->carte->interieur, i, j, "S");
                wrefresh(interface->infos->interieur);
                wattroff(interface->carte->interieur, COLOR_PAIR(COULEUR_UNITE));
            }
            else if(jeu->carte[i][j] == CASE_COMMANDO) {
                waddch(interface->carte->interieur, 'C' | COLOR_PAIR(COULEUR_UNITE));
            }
            else if(jeu->carte[i][j] == CASE_VEHICULE) {
                waddch(interface->carte->interieur, 'V' | COLOR_PAIR(COULEUR_UNITE));
            }
            else if(jeu->carte[i][j] == CASE_MISSILE) {
                waddch(interface->carte->interieur, 'M' | COLOR_PAIR(COULEUR_UNITE));
            }
            else if(jeu->carte[i][j] == CASE_CHAR) {
                waddch(interface->carte->interieur, 'T' | COLOR_PAIR(COULEUR_UNITE));
            }
            else if(jeu->carte[i][j] == CASE_VIDE) {
                waddch(interface->carte->interieur, ' ' | COLOR_PAIR(COULEUR_VIDE));
            }
            else if((jeu->carte[i][j] >= CASE_MIN_JOUEUR) && (jeu->carte[i][j] <= CASE_MAX_JOUEUR)) {
                wattron(interface->carte->interieur, COLOR_PAIR(COULEUR_JOUEUR));
                wprintw(interface->carte->interieur, "%d", jeu->carte[i][j]);
                wattroff(interface->carte->interieur, COLOR_PAIR(COULEUR_JOUEUR));
            }
            else if(jeu->carte[i][j] == CASE_ORDI) {
                wattron(interface->carte->interieur, COLOR_PAIR(COULEUR_ORDI));
                wprintw(interface->carte->interieur, "O");
                wattroff(interface->carte->interieur, COLOR_PAIR(COULEUR_ORDI));
            }
            else if(jeu->carte[i][j] == CASE_FORT) {
                waddch(interface->carte->interieur, 'F' | COLOR_PAIR(COULEUR_FORT));
            }
            else if((jeu->carte[i][j] >= CASE_MIN_CHEMIN) && (jeu->carte[i][j] <= CASE_MAX_CHEMIN)) {
                waddch(interface->carte->interieur, ' ' | COLOR_PAIR(COULEUR_CHEMIN));
            }
        }
    }
}

/**
 * Affichage de la fenetre etat
 * @param[out] interface (interface_t *)
 * @param[out] jeu (jeu_t *)
 **/
void affichage_etat(interface_t *interface, jeu_t *jeu){
    int *id_adv = NULL;

    id_adv = recuperer_indice_adv(jeu);
    wrefresh(interface->carte->interieur);
    interface->etat = fenetre_creer(ETAT_POSX, ETAT_POSY, ETAT_LARGEUR, ETAT_HAUTEUR, "Etat", FALSE);
    wprintw(interface->etat->interieur, "Vies      %2d\n", jeu->vies);
    wprintw(interface->etat->interieur, "Argent %5d\n", jeu->argent);
    wprintw(interface->etat->interieur, "Freeze     0\n",jeu->freeze);
    wprintw(interface->etat->interieur, "Unfreeze   0\n",jeu->unfreeze);
    wprintw(interface->etat->interieur, "Adv. %d    %d\n",id_adv[0], jeu->adv[0]);
    wprintw(interface->etat->interieur, "Adv. %d    %d\n",id_adv[1], jeu->adv[1]);
    wprintw(interface->etat->interieur, "Adv. %d    %d\n",id_adv[2], jeu->adv[2]);    
    wrefresh(interface->etat->interieur);
}