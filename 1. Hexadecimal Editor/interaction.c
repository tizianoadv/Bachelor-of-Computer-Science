/**
 * INTERACTIONS
 * Fichier regroupant les fonctions qui permettent la gestion de l'interface homme / machine,
 * Ainsi que l'affichage dynamique des éléments.
 * @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */ 

#include <stdlib.h>
#include <ncurses.h>

#include "fichier.h"
#include "interface.h"
#include "ncurses.h"
#include "interaction.h"

/**
 * Ecriture de message dans la fenetre 1.
 * @param[out] fenetre1 la sous fenetre 1 de la fenetre parent 1
 * @param[out] indice_message indice représentant un type de message à afficher
 * @param[out] position position l'octet selectionné dans un bloc, relatif à clique gauche
 * @param[out] indice_fenetre indice représtant le numéro de la fenetre (2 ou 3)
 * @param[out] num_bloc le numero du bloc affiché 
 * @param[out] new_octet le nouvel octet saisi par l'utilisateur
 * @param[out] taille_fichier la taille max actuelle du fichier 
 */
void message_fenetre1(WINDOW *fenetre1, int indice_message, int position, int indice_fenetre, int num_bloc,unsigned char* new_octet, int taille_fichier){
   wattron(fenetre1, COLOR_PAIR(1));

  switch (indice_message)
  {
    case 1 :
      wprintw(fenetre1,"Fichier chargé !\n");
    break;

    case 2 :
      if( position != -1 && indice_fenetre == 2){
        wprintw(fenetre1,"Vous avez selectionné l'octet %d (valeur = %X)      \n", (position+1)+(TAILLE_BLOC*num_bloc), TAMPON[position]);
      }
      else if( position != -1 && indice_fenetre == 3 ){
        wprintw(fenetre1,"Vous avez selectionné l'octet %d (valeur = %c)      \n", (position+1)+(TAILLE_BLOC*num_bloc), TAMPON[position]);
      }
      else{
        ncurses_stopper();
        fprintf(stderr," Erreur indice fenetre inconnu\n");
        exit(EXIT_FAILURE);
      }
    break;

    case 3 :
      wprintw(fenetre1,"Veuillez saisir un caractère (nouvel octet) :         \n");
    break;

    case 4 :
      if( new_octet != NULL ){
        wprintw(fenetre1,"Vous avez saisie : %c         \n",*new_octet);
      }
    break;

    case 5 :
      wprintw(fenetre1,"Fichier modifié !      \n");
    break;

    case 6 :
      wprintw(fenetre1,"Fichier tronqué !\n");
      wprintw(fenetre1,"Nouvelle taille : %d octets\n", taille_fichier);
    break;
  
    default:
      ncurses_stopper();
      fprintf(stderr," Erreur indice message inconnu\n");
      exit(EXIT_FAILURE);
  }
  wattroff(fenetre1, COLOR_PAIR(1));
  wrefresh(fenetre1);
}

/**
 * Affichage fenetre 2 et 3
 * Remplissage de la fenetre 2.
 * - Partie lignes
 *  -> affichage de lignes en héxadécimal
 * - Partie héxa
 *  -> Affichage des octets contenu dans un bloc en héxadécimal
 * Remplissage de la fenetre 3.
 *  ->Affichgage des octets contenu dans un bloc en format caractère 
 * @param[out] fd file descriptor du fichier
 * @param[out] fenetre2 la sous fenetre de la fenetre parent 2
 * @param[out] fenetre3 la sous fenetre de la fenetre parent 3
 * @param[out] num_bloc le numero du bloc à affiché
 * @return le nombre d'octets lu est placé dans le TAMPON
 */
int remplissage_fenetre23(int* fd, WINDOW* fenetre2, WINDOW* fenetre3, int num_bloc){
  int i,y,x, num_octet, nb_octet_tampon;
  int num_ligne=0;

  /* vindage fenetre 2 et 3*/
  wrefresh(fenetre2);
  wrefresh(fenetre3);

  /* Lecture du fichier et mise en tampon*/
  nb_octet_tampon = lecture_nieme_bloc(&fd,TAMPON,num_bloc);
  
  wattron(fenetre2, COLOR_PAIR(1));
  wattron(fenetre3, COLOR_PAIR(1));

  /* Remplissage fenetre 2 -> numero lignes */ 
  for( y=0, num_ligne=0 ; y < ((TAILLE_TAMPON/8) + 1) ; y++ ) {
    num_ligne=y;
    if( nb_octet_tampon == 128 )
      num_ligne += (TAILLE_TAMPON/8)*num_bloc;
    else{
      if( y < ((nb_octet_tampon/8) + 1))
        num_ligne += (TAILLE_TAMPON/8)*num_bloc + ( (nb_octet_tampon/8) + 1 );
      else
        num_ligne=0;
    }
    mvwprintw(fenetre2, y , 0, "%08X", num_ligne * 8) ;
  }

  /* Remplissage fenetre 2 -> valeur hexa */
  for( y=0, num_octet=0 ; y < HAUTEUR_F2 ; y++ ){

    for( i=0, x=8 ; i < 8 ; i++, x++, num_octet++){

      mvwprintw(fenetre2, y, x, " ");
      x++;
        
      /* On affiche '00' quand il n'y a plus d'octet à lire */
      if( ( num_octet < nb_octet_tampon ) && ( est_affichable( *(TAMPON + (num_octet) ) ) ) ){
        mvwprintw(fenetre2, y, x, "%X", *(TAMPON + (num_octet)) );
        x++;
      }
      else{
        mvwprintw(fenetre2, y, x, "00");
        x++;
      }
      wrefresh(fenetre2);
    }
  }

  /* Remplissage fenetre 3 */
  for( y=0, num_octet=0; y < HAUTEUR_F3 ; y++){
    for( x=0; x < 8 ; x++, num_octet++ ){

      if( (num_octet < nb_octet_tampon) && ( est_affichable( *(TAMPON + (num_octet) ) ) ) ){
        mvwprintw(fenetre3, y , x , "%c",*(TAMPON + (num_octet)) );
      }
      else{
        mvwprintw(fenetre3, y , x , ".");
      }

      wrefresh(fenetre3);
    }
  }

  wattroff(fenetre2, COLOR_PAIR(1));
  wattroff(fenetre3, COLOR_PAIR(1));

  return nb_octet_tampon;
}

/**
 * Fonction necessite d'être opitimiser !
 * Determine le numero de la colonne des "00" dans la fenetre 2
 * @param[out] x coordonnée de la colonne lors du clique gauche
 * @return le numero de la colonne
 */
int identification_num_col_f2(int x){
  int num_col=-1;
  switch (x)
  {
    case 10:
      num_col=0;
      break;
    case 11:
      num_col=0;
      break;
    case 13:
      num_col=1;
      break;
    case 14:
      num_col=1;
      break;
    case 16:
      num_col=2;
      break;
    case 17:
      num_col=2;
      break;
    case 19:
      num_col=3;
      break;
    case 20:
      num_col=3;
      break;
    case 22:
      num_col=4;
      break;
    case 23:
      num_col=4;
      break;
    case 25:
      num_col=5;
      break;
    case 26:
      num_col=5;
      break;
    case 28:
      num_col=6;
      break;
    case 29:
      num_col=6;
      break;
    case 31:
      num_col=7;
      break;
    case 32:
      num_col=7;
      break;
  default:
    num_col=-1;
    break;
  }
  return num_col;
}

/**
 * Change l'affichage de l'octet sélectionné
 * @param[out] indice_fenetre indice de la fenetre 2 ou 3.
 * @param[out] y coordonnée de hauteur du clique gauche
 * @param[out] x coordonnée de largeur du clique gauche
 * @param[out] indice_fenetre l'indice de la fenetre ( 2 ou 3 )
 */ 
void coloration_indice(WINDOW* fenetre, int y, int x, int indice_fenetre){

  wattron(fenetre,COLOR_PAIR(2));

  if(indice_fenetre == 2 ){
      mvwprintw(fenetre, y, (x*3) + 9, "%X", mat_tampon[y][x] );
  }
  else if (indice_fenetre == 3){
      mvwprintw(fenetre, y, x, "%c", mat_tampon[y][x] );
  }
  else {
    ncurses_stopper();
    fprintf(stderr," Erreur indice fenetre inconnu\n");
    exit(EXIT_FAILURE);
  }

  wattroff(fenetre,COLOR_PAIR(2));
  wrefresh(fenetre);
}

/**
 * Lecture du nouvel octet
 * @param[out] new_octet nouvel octet saisie par l'utilisateur
 */
void lecture_saisie(unsigned char* new_octet){
  if( scanf("%1c",new_octet) == EOF ){
      ncurses_stopper();
      perror("Erreur du scanf ");
      exit(EXIT_FAILURE);
    }
}

/**
 * Determination si l'octet est affichable ou non
 * Affichage Letres minusucules, majuscules et chiffres  : 
 *  (octet >= 48 && octet <= 57) => Chiffres
 *  (octet >= 65 && octet <= 90) => Majuscules
 *  (octet >= 97 && octet <= 122) => Minuscules
 * @param[out] octet octet représentant le caractère
 */
int est_affichable(unsigned char octet){
  int resultat=0;
  if( (octet >= 48 && octet <= 57) ||
      (octet >= 65 && octet <= 90) ||
      (octet >= 97 && octet <= 122)  )
    resultat=1;
    
  return resultat;
} 