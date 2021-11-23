/**
 * INTERFACE
 * Fichier regroupant les fonctions qui permettent l'affichage des statique des éléments 
 * @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */ 

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <stdarg.h>
#include <stdlib.h>
#include <ncurses.h>

#include "ncurses.h"
#include "interface.h"
#include "fichier.h"

/**
 * Creation de fenetre parent.
 * @param[out] indice_fenetre l'indice de la fenetre 1, 2 ou 3
 * @return fenetre_parent (WINDOW *)
 */
WINDOW* creation_fenetre_parent(int indice_fenetre){
  WINDOW* fenetre_parent;
  
  switch (indice_fenetre)
  {
    case 1:
      fenetre_parent = newwin(HAUTEUR_F1P, LARGEUR_F1P, POSY_F1P,POSX_F1P);
      break;
    case 2:
      fenetre_parent = newwin(HAUTEUR_F2P, LARGEUR_F2P, POSY_F2P,POSX_F2P);
      break;
    case 3:
      fenetre_parent = newwin(HAUTEUR_F3P, LARGEUR_F3P, POSY_F3P,POSX_F3P);
      break;
    default:
      ncurses_stopper();
      fprintf(stderr, "Erreur indice fenetre inconnu.\n");
      exit(EXIT_FAILURE);
  }
  return fenetre_parent;
}

/**
 * Creation de fenetre (sous fenetre de fenetre parent).
 * @param[out] fenetre_parent fenetre parent de la fenetre à créer
 * @param[out] indice_fenetre l'indice de la fenetre 1, 2 ou 3
 * @return fenetre (WINDOW *)
 */
WINDOW* creation_fenetre(WINDOW* fenetre_parent,int indice_fenetre){
  WINDOW* fenetre;
  
  switch (indice_fenetre)
  {
    case 1:
      fenetre = subwin(fenetre_parent, HAUTEUR_F1,LARGEUR_F1,POSY_F1,POSX_F1);
      break;
    case 2:
      fenetre = subwin(fenetre_parent, HAUTEUR_F2,LARGEUR_F2,POSY_F2,POSX_F2); 
      break;
    case 3:
      fenetre = subwin(fenetre_parent, HAUTEUR_F3,LARGEUR_F3,POSY_F3,POSX_F3);
      break;
    default:
      ncurses_stopper();
      fprintf(stderr, "Erreur indice fenetre inconnu.\n");
      exit(EXIT_FAILURE);
  }
  return fenetre;
}

/**
 * Création et initialisation de l'interface de la fenetre.
 * @param[out] fenetre_parent la fenetre parent
 * @param[out] fenetre la sous fenetre de la fenetre parent
 * @param[out] indice_fenetre l'indice de la fenetre 1, 2 ou 3
 */
void creation_interface(WINDOW* fenetre_parent, WINDOW* fenetre, int indice_fenetre){

  switch (indice_fenetre) {
      /* Cas fenetre 1*/
      case 1:
        /* Création interface parent */
        box(fenetre_parent, 0, 0);
        wbkgd(fenetre_parent, COLOR_PAIR(1));
        mvwprintw(fenetre_parent,0,2," Informations ");
        
        /* Création interface fenetre */
        wbkgd(fenetre, COLOR_PAIR(3));
        scrollok(fenetre, TRUE);

        /* Rafraichissement */
        wrefresh(fenetre_parent);
        wrefresh(fenetre);
      break;
      
      /* Cas fenetre 2*/
      case 2:
        /* Création interface fenetre parent */
        box(fenetre_parent, 0, 0);
        wbkgd(fenetre_parent, COLOR_PAIR(1));
        mvwprintw(fenetre_parent,0,2," Hexa ");

        /* Création interface fenetre */    
        wbkgd(fenetre, COLOR_PAIR(3));

        /* Rafraichissement */
        wrefresh(fenetre_parent);
        wrefresh(fenetre);
      break;
      
      /* Cas fenetre 3*/
      case 3:
        /* Création interface fenetre parent */
        box(fenetre_parent, 0, 0);
        wbkgd(fenetre_parent, COLOR_PAIR(1 ));
        mvwprintw(fenetre_parent,0,1," Car ");

        /* Création interface fenetre */
        wbkgd(fenetre, COLOR_PAIR(3));

        /* Rafraichissement */
        wrefresh(fenetre_parent);
        wrefresh(fenetre);
      break;

      /* Cas erreur */
      default:
      ncurses_stopper();
      fprintf(stderr, "Erreur indice fenetre inconnu.\n");
      exit(EXIT_FAILURE);
  }
}

/**
 * Supression de fenetre.
 * @param[out] fenetre_parent la fenetre parent
 * @param[out] fenetre la sous fenetre de la fenetre parent
 */
void suppresion_fenetre(WINDOW* fenetre_parent, WINDOW* fenetre ){
    /* Suppression de la fenêtre */
    delwin(fenetre);
    /* Suppression de la fenêtre parent*/
    delwin(fenetre_parent);    
}
