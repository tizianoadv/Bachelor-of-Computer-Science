#include "fenetre.h"
#include "ncurses.h"
#include "jeu.h"
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h> /* pour sleep */

#include "interface.h"



/* Récupération des indices des adversaires */
int* recuperer_indice_adv(jeu_t *jeu){
    int i,j,k;
    int *retour = NULL;
    int taille_tab=3;
    int tmp;

    if( (retour =(int*)malloc(sizeof(int)*3)) == NULL ){
        perror("Erreur allocation recuperer_indice_adv");
        exit(EXIT_FAILURE);
    }

    /* On recupere les indices des adv*/
    for(i=0,k=0;i<15;i++){
        for(j=0;j<15;j++){
            if( jeu->carte[i][j] > 0 && jeu->carte[i][j] <= 4 ){retour[k] = jeu->carte[i][j]; k++;}
        }
    }
    if( retour[0] == 4 && retour[1] == 4 && retour[2] == 4){retour[0]=1;retour[1]=2;retour[2]=3;}
    /* Trie */
    while( taille_tab > 1 ){
        
        for( i=0 ; i <  taille_tab -1 ; i++ ){

            if( retour[i] > retour[i+1] ){
                tmp = retour[i];
                retour[i] = retour[i+1];
                retour[i+1] = tmp;
            }
        }
        taille_tab--;
    }
    return retour;
}

/**
 * Définition de la palette.
 */
void palette() {
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_RED);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);
}

/* Création de l'interface de démarrage */
interface_t creer_interface_demarrage(){
    interface_t retour;
    int ch;
    bool continuer = FALSE;
    
    retour.infos = fenetre_creer(INFOS_POSX, INFOS_POSY, INFOS_LARGEUR, INFOS_HAUTEUR, "Informations", TRUE);
    wprintw(retour.infos->interieur, "\t\t* Tower Defense *\n");
    wprintw(retour.infos->interieur, "\t@Tiziano NARDONE & @Christian RAHAL\n");
    wprintw(retour.infos->interieur, "- Meilleur joueur : Un dénommé Cyril\n");
    wprintw(retour.infos->interieur, "- Finisher : Rabattaque\n"); 
    wprintw(retour.infos->interieur, "-> Appuyer sur 'c' pour commencer ...\n"); 
    retour.outils = fenetre_creer(OUTILS_POSX, OUTILS_POSY, OUTILS_LARGEUR, OUTILS_HAUTEUR, "Outils", FALSE);
    retour.etat = fenetre_creer(ETAT_POSX, ETAT_POSY, ETAT_LARGEUR, ETAT_HAUTEUR, "Etat", FALSE);
    retour.carte = fenetre_creer(CARTE_POSX, CARTE_POSY, CARTE_LARGEUR, CARTE_HAUTEUR, "Carte", FALSE);
    
    wrefresh(retour.infos->interieur);
    while(continuer == FALSE){
        ch = getch();
        if((ch == 'C') || (ch == 'c'))
            continuer = true;
        else{
            wprintw(retour.infos->interieur, "-> Appuyer sur 'c' pour commencer ...\n");
            wrefresh(retour.infos->interieur);
        }
    }
    continuer = FALSE;

    wrefresh(retour.etat->interieur);
    return retour;
}

/**
 * Création d'une interface.
 * @param jeu l'état du jeu
 * @return l'interface créée
 */
interface_t* interface_creer(jeu_t *jeu) {
    int i,j;
    int *id_adv = NULL;
    interface_t *retour = NULL;
    
    if( (retour = (interface_t*) malloc(sizeof(interface_t))) == NULL ){
        perror("Erreur malloc new interface");
        exit(EXIT_FAILURE);
    }

    id_adv = recuperer_indice_adv(jeu);
  
    retour->outilsel = OUTIL_NONE;

    /*retour.infos = fenetre_creer(INFOS_POSX, INFOS_POSY, INFOS_LARGEUR, INFOS_HAUTEUR, "Informations", TRUE);
    wprintw(retour.infos->interieur, "Tapez 'Q' pour quitter");
    wprintw(retour.infos->interieur, "\n%d, %d, %d pour retirer des vies aux adversaires",id_adv[0],id_adv[1],id_adv[2]);
    wprintw(retour.infos->interieur, "\nV pour retirer des vies a l'utilisateur");
    wprintw(retour.infos->interieur, "\n$ pour ajouter de l'argent");
    wprintw(retour.infos->interieur, "\nF et U pour augmenter freeze et unfreeze");
    wrefresh(retour.infos->interieur);*/

    retour->outils = fenetre_creer(OUTILS_POSX, OUTILS_POSY, OUTILS_LARGEUR, OUTILS_HAUTEUR, "Outils", FALSE);
    interface_MAJOutils(retour, jeu);

    retour->etat = fenetre_creer(ETAT_POSX, ETAT_POSY, ETAT_LARGEUR, ETAT_HAUTEUR, "Etat", FALSE);
    wprintw(retour->etat->interieur, "Vies      %2d\n", jeu->vies);
    wprintw(retour->etat->interieur, "Argent %5d\n", jeu->argent);
    wprintw(retour->etat->interieur, "Freeze     0\n",jeu->freeze);
    wprintw(retour->etat->interieur, "Unfreeze   0\n",jeu->unfreeze);
    wprintw(retour->etat->interieur, "Adv. %d    %d\n",id_adv[0], jeu->adv[0]);
    wprintw(retour->etat->interieur, "Adv. %d    %d\n",id_adv[1], jeu->adv[1]);
    wprintw(retour->etat->interieur, "Adv. %d    %d\n",id_adv[2], jeu->adv[2]);    
    wrefresh(retour->etat->interieur);
    
    retour->attaques = fenetre_creer(ATTAQUES_POSX, ATTAQUES_POSY, ATTAQUES_LARGEUR, ATTAQUES_HAUTEUR, "Attaques", FALSE);
    interface_MAJAttaques(retour, jeu,id_adv[0],id_adv[1],id_adv[2]);
    
    retour->carte = fenetre_creer(CARTE_POSX, CARTE_POSY, CARTE_LARGEUR, CARTE_HAUTEUR, "Carte", FALSE);
    for(i = 0; i < 15; i++) {
        for(j = 0; j < 15; j++) {
            if(jeu->carte[i][j] == CASE_VIDE) {
                waddch(retour->carte->interieur, ' ' | COLOR_PAIR(COULEUR_VIDE));
            }
            else if((jeu->carte[i][j] >= CASE_MIN_JOUEUR) && (jeu->carte[i][j] <= CASE_MAX_JOUEUR)) {
                wattron(retour->carte->interieur, COLOR_PAIR(COULEUR_JOUEUR));
                wprintw(retour->carte->interieur, "%d", jeu->carte[i][j]);
            }
            else if(jeu->carte[i][j] == CASE_ORDI) {
                wattron(retour->carte->interieur, COLOR_PAIR(COULEUR_ORDI));
                wprintw(retour->carte->interieur, "O");
            }
            else if(jeu->carte[i][j] == CASE_FORT) {
                waddch(retour->carte->interieur, 'F' | COLOR_PAIR(COULEUR_FORT));
            }
            else if((jeu->carte[i][j] >= CASE_MIN_CHEMIN) && (jeu->carte[i][j] <= CASE_MAX_CHEMIN)) {
                waddch(retour->carte->interieur, ' ' | COLOR_PAIR(COULEUR_CHEMIN));
            }
        }
    }
    wrefresh(retour->carte->interieur);
    return retour;
}

/**
 * Suppression d'une interface.
 * @param interface l'interface à supprimer
 */
void interface_supprimer(interface_t *interface) {
    fenetre_supprimer(&interface->infos);
    fenetre_supprimer(&interface->outils);
    fenetre_supprimer(&interface->etat);
    fenetre_supprimer(&interface->carte);
    fenetre_supprimer(&interface->attaques);
}

/**
 * Met à jour la fenêtre 'Attaques'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJAttaques(interface_t *interface, jeu_t *jeu, int adv1, int adv2, int adv3){

    werase(interface->attaques->interieur);

    wattron(interface->attaques->interieur, COLOR_PAIR(1));
    if(jeu->argent < SOLDAT_COUT)
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Soldat          %4d$ -> %d%d%d\n", SOLDAT_COUT,adv1,adv2,adv3);    
    
    if(jeu->argent < COMMANDO_COUT)
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Commando        %4d$ -> %d%d%d\n", COMMANDO_COUT,adv1,adv2,adv3);
    
    if(jeu->argent < VEHICULE_COUT)
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Vehicule blinde %4d$ -> %d%d%d\n", VEHICULE_COUT,adv1,adv2,adv3);
    
    if(jeu->argent < MISSILE_COUT)
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Lance-missiles  %4d$ -> %d%d%d\n", MISSILE_COUT,adv1,adv2,adv3);

    if(jeu->argent < CHAR_COUT)
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Char            %4d$ -> %d%d%d\n", CHAR_COUT,adv1,adv2,adv3);
    
    if(jeu->freeze == 10)
        wattron(interface->attaques->interieur, COLOR_PAIR(1));
    else
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Freeze                -> %d%d%d\n",adv1,adv2,adv3);
    wrefresh(interface->attaques->interieur);
}

/**
 * Met à jour la fenêtre 'Outils'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJOutils(interface_t *interface, jeu_t *jeu) {
    werase(interface->outils->interieur);

    if(jeu->argent < TOUR_1_COUT) {
        wattron(interface->outils->interieur, COLOR_PAIR(4));
        if(interface->outilsel == OUTIL_TOUR_1)
            interface->outilsel = OUTIL_NONE;
    }
    else if(interface->outilsel == OUTIL_TOUR_1)
        wattron(interface->outils->interieur, COLOR_PAIR(5));
    else
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Tour 1 %4d$\n", TOUR_1_COUT);

    if(jeu->argent < TOUR_2_COUT) {
        if(interface->outilsel == OUTIL_TOUR_2)
            interface->outilsel = OUTIL_NONE;
        wattron(interface->outils->interieur, COLOR_PAIR(4));
    }
    else if(interface->outilsel == OUTIL_TOUR_2)
        wattron(interface->outils->interieur, COLOR_PAIR(5));
    else
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Tour 2 %4d$\n", TOUR_2_COUT);

    if(jeu->argent < TOUR_3_COUT) {
        if(interface->outilsel == OUTIL_TOUR_3)
            interface->outilsel = OUTIL_NONE;
        wattron(interface->outils->interieur, COLOR_PAIR(4));
    }
    else if(interface->outilsel == OUTIL_TOUR_3)
        wattron(interface->outils->interieur, COLOR_PAIR(5));
    else
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Tour 3 %4d$\n", TOUR_3_COUT);

    if(jeu->argent < TOUR_4_COUT) {
        if(interface->outilsel == OUTIL_TOUR_4)
            interface->outilsel = OUTIL_NONE;
        wattron(interface->outils->interieur, COLOR_PAIR(4));
    }
    else if(interface->outilsel == OUTIL_TOUR_4)
        wattron(interface->outils->interieur, COLOR_PAIR(5));
    else
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Tour 4 %4d$\n", TOUR_4_COUT);

    if(jeu->argent < TOUR_5_COUT) {
        if(interface->outilsel == OUTIL_TOUR_5)
            interface->outilsel = OUTIL_NONE;
        wattron(interface->outils->interieur, COLOR_PAIR(4));
    }
    else if(interface->outilsel == OUTIL_TOUR_5)
        wattron(interface->outils->interieur, COLOR_PAIR(5));
    else
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Tour 5 %4d$\n", TOUR_5_COUT);

    if(jeu->unfreeze < 10)
        wattron(interface->outils->interieur, COLOR_PAIR(4));
    else 
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Unfreeze\n");

    wrefresh(interface->outils->interieur);
}

/**
 * Met à jour la fenêtre 'Etat'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJEtat(interface_t *interface, jeu_t *jeu) {
    mvwprintw(interface->etat->interieur, 0, 8, "%4d", jeu->vies);
    mvwprintw(interface->etat->interieur, 1, 7, "%5d", jeu->argent);
    mvwprintw(interface->etat->interieur, 2, 8, "%4d", jeu->freeze);
    mvwprintw(interface->etat->interieur, 3, 8, "%4d", jeu->unfreeze);
    mvwprintw(interface->etat->interieur, 4, 8, "%4d", jeu->adv[0]);
    mvwprintw(interface->etat->interieur, 5, 8, "%4d", jeu->adv[1]);
    mvwprintw(interface->etat->interieur, 6, 8, "%4d", jeu->adv[2]);
    wrefresh(interface->etat->interieur);
}

/**
 * Gestion des actions dans la fenêtre des outils.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre 
 */
void interface_outils(interface_t *interface, jeu_t *jeu, int posX, int posY) {
    switch(posY) {
        case 0:
            if(jeu->argent >= TOUR_1_COUT) {
                interface->outilsel = OUTIL_TOUR_1;
                interface_MAJOutils(interface, jeu);
                wprintw(interface->infos->interieur, "\nOutil tour 1 selectionne. Cliquez sur un carre d'herbe.");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'argent pour acheter une tour 1.");
            wrefresh(interface->infos->interieur);
        break;
        case 1:
            if(jeu->argent >= TOUR_2_COUT) {
                interface->outilsel = OUTIL_TOUR_2;
                interface_MAJOutils(interface, jeu);
                wprintw(interface->infos->interieur, "\nOutil tour 2 selectionne. Cliquez sur un carre d'herbe.");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'argent pour acheter une tour 2.");
            wrefresh(interface->infos->interieur);
        break;
        case 2:
            if(jeu->argent >= TOUR_3_COUT) {
                interface->outilsel = OUTIL_TOUR_3;
                interface_MAJOutils(interface, jeu);
                wprintw(interface->infos->interieur, "\nOutil tour 3 selectionne. Cliquez sur un carre d'herbe.");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'argent pour acheter une tour 3.");
            wrefresh(interface->infos->interieur);
        break;
        case 3:
            if(jeu->argent >= TOUR_4_COUT) {
                interface->outilsel = OUTIL_TOUR_4;
                interface_MAJOutils(interface, jeu);
                wprintw(interface->infos->interieur, "\nOutil tour 4 selectionne. Cliquez sur un carre d'herbe.");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'argent pour acheter une tour 4.");
            wrefresh(interface->infos->interieur);
        break;
        case 4:
            if(jeu->argent >= TOUR_5_COUT) {
                interface->outilsel = OUTIL_TOUR_5;
                interface_MAJOutils(interface, jeu);                        
                wprintw(interface->infos->interieur, "\nOutil tour 5 selectionne. Cliquez sur un carre d'herbe.");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'argent pour acheter une tour 5.");
            wrefresh(interface->infos->interieur);
        break;
        case 5:
            if(jeu->unfreeze == 10) {
                interface->outilsel = OUTIL_NONE;
                jeu->unfreeze = 0;
                interface_MAJOutils(interface, jeu);
                interface_MAJEtat(interface, jeu);
                wprintw(interface->infos->interieur, "\nUnfreeze lance... pour de faux !");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'energie pour lancer un unfreeze.");
            wrefresh(interface->infos->interieur);
        break;
    }
}

/**
 * Gestion des actions dans la fenêtre des attaques.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre 
 */
void interface_attaques(interface_t *interface, jeu_t *jeu, int posX, int posY) {
    if((posX >= 25) && (posX <= 27)) {
        switch(posY) {
            case 0:
                if(jeu->argent < SOLDAT_COUT)
                    wprintw(interface->infos->interieur, "\nPas assez d'argent pour lancer un soldat.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->argent -= SOLDAT_COUT;
                    interface_MAJAttaques(interface, jeu,0,0,0);
                    interface_MAJEtat(interface, jeu);
                    interface_MAJOutils(interface, jeu);
                    wprintw(interface->infos->interieur, "\nEnvoi d'un soldat à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
            case 1:
                if(jeu->argent < COMMANDO_COUT)
                    wprintw(interface->infos->interieur, "\nPas assez d'argent pour lancer un commando.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->argent -= COMMANDO_COUT;
                    interface_MAJAttaques(interface, jeu,0,0,0);
                    interface_MAJEtat(interface, jeu);
                    interface_MAJOutils(interface, jeu);
                    wprintw(interface->infos->interieur, "\nEnvoi d'un commando à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
            case 2:
                if(jeu->argent < VEHICULE_COUT)
                    wprintw(interface->infos->interieur, "\nPas assez d'argent pour lancer un vehicule blinde.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->argent -= VEHICULE_COUT;
                    interface_MAJAttaques(interface, jeu,0,0,0);
                    interface_MAJEtat(interface, jeu);
                    interface_MAJOutils(interface, jeu);
                    wprintw(interface->infos->interieur, "\nEnvoi d'un vehicule blinde à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
            case 3:
                if(jeu->argent < MISSILE_COUT)
                    wprintw(interface->infos->interieur, "\nPas assez d'argent pour lancer un lance-missiles.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->argent -= MISSILE_COUT;
                    interface_MAJAttaques(interface, jeu,0,0,0);
                    interface_MAJEtat(interface, jeu);
                    interface_MAJOutils(interface, jeu);
                    wprintw(interface->infos->interieur, "\nEnvoi d'un lance-missiles à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
            case 4:
                if(jeu->argent < CHAR_COUT)
                    wprintw(interface->infos->interieur, "\nPas assez d'argent pour lancer un char.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->argent -= CHAR_COUT;
                    interface_MAJAttaques(interface, jeu,0,0,0);
                    interface_MAJEtat(interface, jeu);
                    interface_MAJOutils(interface, jeu);
                    wprintw(interface->infos->interieur, "\nEnvoi d'un char à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
            case 5:
                if(jeu->freeze < 10)
                    wprintw(interface->infos->interieur, "\nPas assez d'énergie pour lancer un freeze.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->freeze = 0;
                    interface_MAJAttaques(interface, jeu,0,0,0);
                    interface_MAJEtat(interface, jeu);
                    wprintw(interface->infos->interieur, "\nEnvoi d'un freeze à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
        }
        wrefresh(interface->infos->interieur);
    }
}

/**
 * Gestion des actions dans la fenêtre de la carte.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre
 */
void interface_carte(interface_t *interface, jeu_t *jeu, int posX, int posY) {
    switch(interface->outilsel) {
        case OUTIL_NONE:
            /* Pas d'outils sélectionné : on affiche le contenu de la case */
            if(jeu->carte[posY][posX] == CASE_VIDE) {
                wprintw(interface->infos->interieur, "\nOh !!! De l'herbe !!!");
            }
            else if((jeu->carte[posY][posX] >= CASE_MIN_JOUEUR) && (jeu->carte[posY][posX] <= CASE_MAX_JOUEUR)) {
                wprintw(interface->infos->interieur, "\nLe point de depart des unites de l'adversaire %d", (jeu->carte[posY][posX] - CASE_MIN_JOUEUR + 1));
            }
            else if(jeu->carte[posY][posX] == CASE_ORDI) {
                wprintw(interface->infos->interieur, "\nLe point de depart des vagues envoyees par l'ordinateur");
            }
            else if(jeu->carte[posY][posX] == CASE_FORT) {
                wprintw(interface->infos->interieur, "\nLe fort a proteger");
            }
            else if((jeu->carte[posY][posX] >= CASE_MIN_CHEMIN) && (jeu->carte[posY][posX] <= CASE_MAX_CHEMIN)) {
                wprintw(interface->infos->interieur, "\nUne route...");
            }
            break;
        case OUTIL_TOUR_1:
            if((jeu->carte[posY][posX] == CASE_VIDE) && (jeu->argent >= TOUR_1_COUT)) {
                jeu->argent -= TOUR_1_COUT;
                wprintw(interface->infos->interieur, "\nTour 1 posee... pour de faux !");
                interface_MAJOutils(interface, jeu);
                interface_MAJEtat(interface, jeu);
                interface_MAJAttaques(interface, jeu,0,0,0);
            }
            else {
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
        case OUTIL_TOUR_2:
            if((jeu->carte[posY][posX] == CASE_VIDE) && (jeu->argent >= TOUR_2_COUT)) {
                jeu->argent -= TOUR_2_COUT;
                wprintw(interface->infos->interieur, "\nTour 2 posee... pour de faux !");
                interface_MAJOutils(interface, jeu);
                interface_MAJEtat(interface, jeu);
                interface_MAJAttaques(interface, jeu,0,0,0);
            }
            else {
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
        case OUTIL_TOUR_3:
            if((jeu->carte[posY][posX] == CASE_VIDE) && (jeu->argent >= TOUR_3_COUT)) {
                jeu->argent -= TOUR_3_COUT;
                wprintw(interface->infos->interieur, "\nTour 3 posee... pour de faux !");
                interface_MAJOutils(interface, jeu);
                interface_MAJEtat(interface, jeu);
                interface_MAJAttaques(interface, jeu,0,0,0);
            }
            else {
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
        case OUTIL_TOUR_4:
            if((jeu->carte[posY][posX] == CASE_VIDE) && (jeu->argent >= TOUR_4_COUT)) {
                jeu->argent -= TOUR_4_COUT;
                wprintw(interface->infos->interieur, "\nTour 4 posee... pour de faux !");
                interface_MAJOutils(interface, jeu);
                interface_MAJEtat(interface, jeu);
                interface_MAJAttaques(interface, jeu,0,0,0);
            }
            else {
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
        case OUTIL_TOUR_5:
            if((jeu->carte[posY][posX] == CASE_VIDE) && (jeu->argent >= TOUR_5_COUT)) {
                jeu->argent -= TOUR_5_COUT;
                wprintw(interface->infos->interieur, "\nTour 5 posee... pour de faux !");
                interface_MAJOutils(interface, jeu);
                interface_MAJEtat(interface, jeu);
                interface_MAJAttaques(interface, jeu,0,0,0);
            }
            else {
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
        case OUTIL_UNFREEZE:
            if(jeu->freeze == 10) {
                jeu->freeze = 0;
                wprintw(interface->infos->interieur, "\nFREEEEEZE !!!");
                interface_MAJEtat(interface, jeu);
            }
            else {
                interface->outilsel = OUTIL_NONE;
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            interface_MAJOutils(interface, jeu);
            break;
    }
    wrefresh(interface->infos->interieur);
}

/**
 * Gestion des actions de l'utilisateur dans l'interface.
 * @param interface l'interface
 * @param jeu les paramètres de la partie
 * @param c la touche pressée
 */
void interface_main(interface_t *interface, jeu_t *jeu, int c) {
    int sourisX, sourisY, posX, posY;

    if((c == KEY_MOUSE) && (souris_getpos(&sourisX, &sourisY, NULL) == OK)) {
        /* Gestion des actions de la souris */

        if(fenetre_getcoordonnees(interface->outils, sourisX, sourisY, &posX, &posY)) {
            interface_outils(interface, jeu, posX, posY);
        }
        else if(fenetre_getcoordonnees(interface->attaques, sourisX, sourisY, &posX, &posY)) {
            interface_attaques(interface, jeu, posX, posY);
        }
        else if(fenetre_getcoordonnees(interface->carte, sourisX, sourisY, &posX, &posY)) {
            interface_carte(interface, jeu, posX, posY);
        }
    }
    else {
        /* Gestion du clavier : à modifier pour le projet */
        switch(c) {
            case '1':
            case '2':
            case '3':
                /* Supprime une vie à un adversaire */
                if(jeu->adv[c - '1'] != 0) {
                    jeu->adv[c - '1']--;
                    interface_MAJEtat(interface, jeu);
                }
                break;
            case 'V':
            case 'v':
                /* Supprime une vie au joueur */
                if(jeu->vies > 0) {
                    jeu->vies--;
                    interface_MAJEtat(interface, jeu);
                }
                break;
            case '$':
                /* Ajoute de l'argent */
                jeu->argent += 10;
                if(jeu->argent > 99999)
                    jeu->argent = 99999;
                interface_MAJEtat(interface, jeu);
                interface_MAJOutils(interface, jeu);
                interface_MAJAttaques(interface, jeu,0,0,0);
                break;
            case 'U':
            case 'u':
                /* Avance l'état du unfreeze */
                if(jeu->unfreeze < 10) {
                    jeu->unfreeze++;
                    interface_MAJEtat(interface, jeu);
                    interface_MAJOutils(interface, jeu);
                }
                break;
            case 'F':
            case 'f':
                /* Avance l'état du freeze */
                if(jeu->freeze < 10) {
                    jeu->freeze++;
                    interface_MAJEtat(interface, jeu);
                    interface_MAJAttaques(interface, jeu,0,0,0);
                }
                break;
            case 27:
                /* Touche ECHAP : annule l'outil sélectionné */
                interface->outilsel = OUTIL_NONE;
                interface_MAJOutils(interface, jeu);
                break;
            default:
                /* Utile en mode DEBUG si on souhaite afficher le caractère associé à la touche pressée */
                wprintw(interface->infos->interieur, "\nTouche %d pressee", c);
                wrefresh(interface->infos->interieur);
                break;
        }
    }
}
