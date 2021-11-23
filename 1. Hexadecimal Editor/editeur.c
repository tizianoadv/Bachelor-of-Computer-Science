/**
 * Projet 1 : Editeur héxadécimal.
 * MAIN
 * Ce code permet d'afficher les octets contenu dans un fichier et ils peuvent également être modifiés ou surprimmés. 
 * Les octets sont présentés dans des blocs de maximum 128 octets en héxadécimal et en caractère.
 * Pour afficher le bloc suivant -> Appuyer sur la flèche du bas.
 * Pour afficher le bloc précédent -> Appuyer sur la flèche du haut.
 * Pour selectionner un octet -> Appuyer sur le clique gauche de la souris.
 * Pour modifier l'octet selectionné -> Appuyer sur la touche "entré" du clavier et taper un caractère.
 * Pour supprimer l'octet selectionné -> Appuyer sur la touche "s" de gauche.
 * @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */

#include <stdlib.h>
#include <ncurses.h>  

#include "interface.h"
#include "ncurses.h"
#include "fichier.h"
#include "interaction.h"


int main(int argc, char *argv[]){

  int ch;
  int fd;
  int sourisY, sourisX, bouton;
  int num_bloc=0;
  int nb_octet_tampon=-1;
  int nb_max_key_down=-1;
  int position_souris=-1;
  int fenetre_active=0;
  unsigned char new_octet;

  WINDOW* fenetre1_parent; 
  WINDOW* fenetre1;
  WINDOW* fenetre2_parent;
  WINDOW* fenetre2;
  WINDOW* fenetre3_parent;
  WINDOW* fenetre3;

  /* Vérification arguments commande */
  if(argc != 2){
      fprintf(stderr, "Usage: %s nomFichier\n", argv[0]);
      fprintf(stderr, "\tOu:\n");
      fprintf(stderr, "\t\t nomFichier : nom du fichier à ouvrir\n");
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
  if( (LINES < (POSY_F2 + HAUTEUR_F2 + 1) ) || (COLS < (POSX_F3 + LARGEUR_F3 + 1) )) {
    ncurses_stopper();
    fprintf(stderr, 
            "Les dimensions du terminal sont insufisantes : l=%d,h=%d au lieu de l=%d,h=%d\n", 
            COLS, LINES, POSX_F1 + LARGEUR_F1, POSY_F1 + HAUTEUR_F1);
    exit(EXIT_FAILURE);
  }

  /* Création des fenetres parents */
  fenetre1_parent = creation_fenetre_parent(1);
  fenetre2_parent = creation_fenetre_parent(2);
  fenetre3_parent = creation_fenetre_parent(3);

  /* Création des sous fenetres parents appelées fenetres */
  fenetre1 = creation_fenetre(fenetre1_parent,1);
  fenetre2 = creation_fenetre(fenetre2_parent,2);
  fenetre3 = creation_fenetre(fenetre3_parent,3);
  
  /* Création des interfaces des 3 fenetres */
  creation_interface(fenetre1_parent, fenetre1,1);
  creation_interface(fenetre2_parent, fenetre2,2);
  creation_interface(fenetre3_parent, fenetre3,3);

  /**
   * Ouverture du fichier en lecture / ecriture
   * Fichier créé s'il n'existe pas
   * Droits lecture / ecriture utilisateur
   */
  ouverture_fichier(&fd,argv[1]);

  /**
   * Initialisation du fichier 
   * Ecriture dans le fichier de caractère alphabétiques choisit aléatoirement
   * Afin d'obtenir une taille fixé arbitrairement de 266 octets
   */
  initialisation_fichier(&fd);

  /**
   * Recupération de la taille du fichier
   * Fonction necessaire pour connaitre le nombre de "KEY_DOWN" possible
   */
  nb_max_key_down = get_taille_fichier(&fd) / TAILLE_BLOC ;

  /**
   * Affichage du premier bloc 
   * Recupération du nombre d'octet dans le tampon
   */
  wclear(fenetre1);
  wclear(fenetre2);
  wclear(fenetre3);
  nb_octet_tampon = remplissage_fenetre23(&fd, fenetre2, fenetre3, num_bloc);

  /* Mise à jour de la matrice 16 x 8 image du contenu du tampon */
  update_mat_tampon();



  /* Message information : "fichié chargé" */
  message_fenetre1(fenetre1, MESSAGE_1, position_souris, FENETRE_1, num_bloc, &new_octet, get_taille_fichier(&fd));

  /* Tant que l'utilisateur n'appui pas sur F2 
   * Le programme ne s'interromp pas !
   */  
  while( ( ch=getch() ) != KEY_F(2) ){

    switch(ch){

      /* Affichage bloc suivant */
      case KEY_DOWN: 
        if( num_bloc < nb_max_key_down )
          num_bloc++;
        nb_octet_tampon = remplissage_fenetre23(&fd, fenetre2, fenetre3, num_bloc);
        update_mat_tampon();
      break;

      /* Affichage bloc précedent */
      case KEY_UP:
        if(num_bloc > 0)
          num_bloc--;
        nb_octet_tampon = remplissage_fenetre23(&fd, fenetre2, fenetre3, num_bloc);
        update_mat_tampon();
      break;

      /* Selection d'un octet */
      case KEY_MOUSE:

        if(souris_getpos(&sourisY,&sourisX, &bouton) == OK){

          /* Clique dans fenetre 2 (Partie héxa) */
          if(      ( ( sourisY < ( HAUTEUR_F2 + POSY_F2 ) ) &&  ( sourisY >= POSY_F2 ) ) &&
                   ( ( sourisX <= LARGEUR_F2 )              &&  ( sourisX >= 10 )     ) ) {
    
            sourisY-=8;/* Normalise la coordonnée de y entre 0 et 15  */
            sourisX = identification_num_col_f2(sourisX); /*Normalise la coordonnée de x entre 0 et 7 */
            if(sourisX!=-1){
        
              nb_octet_tampon = remplissage_fenetre23(&fd, fenetre2, fenetre3, num_bloc);
              position_souris = coord_to_position(sourisY, sourisX);

              if( position_souris < nb_octet_tampon){

                coloration_indice(fenetre2,sourisY,sourisX,2);
                coloration_indice(fenetre3,sourisY,sourisX,3);
                message_fenetre1(fenetre1, MESSAGE_2, position_souris, FENETRE_2, num_bloc, &new_octet, get_taille_fichier(&fd));
                fenetre_active=1;
              }

            }              
          }

          /* Clique dans fenetre 3 (Partie Car) */
          else if( ( ( sourisY < ( HAUTEUR_F3 + POSY_F3 ) ) && ( sourisY >= POSY_F3 ) ) &&
                   ( ( sourisX < ( 8 + POSX_F3))            && ( sourisX >= POSX_F3 ) ) ){
            
            sourisY-=8;/* Normalise la coordonnée de y entre 0 et 15 */ 
            sourisX -= 36; /* Normalise la coordonnée de x entre 0 et 7 */ 

            nb_octet_tampon = remplissage_fenetre23(&fd, fenetre2, fenetre3, num_bloc);
            position_souris = coord_to_position(sourisY, sourisX);

            if( position_souris < nb_octet_tampon){
              coloration_indice(fenetre3,sourisY,sourisX,3);
              coloration_indice(fenetre2,sourisY,sourisX,2);
              message_fenetre1(fenetre1, MESSAGE_2, position_souris, FENETRE_3, num_bloc, &new_octet, get_taille_fichier(&fd));
              fenetre_active=1;
            }
            
          }

          /* Clique en dehors des fenetres 2 et 3 */
          else{
            remplissage_fenetre23(&fd, fenetre2, fenetre3, num_bloc);
            fenetre_active=0;
          }
  
        }
      break;

      /* Modification octet */ 
      case TOUCHE_ENTRER :

        if(fenetre_active){

          message_fenetre1(fenetre1, MESSAGE_3, position_souris, FENETRE_1, num_bloc, &new_octet, get_taille_fichier(&fd));

          if( (position_souris != -1) || (position_souris != -1) ){
            lecture_saisie(&new_octet);
            message_fenetre1(fenetre1, MESSAGE_4, position_souris, FENETRE_1, num_bloc, &new_octet, get_taille_fichier(&fd));
            modification_octet(&fd, new_octet, position_souris, nb_octet_tampon, num_bloc);
            update_mat_tampon();
            message_fenetre1(fenetre1, MESSAGE_5, position_souris, FENETRE_1, num_bloc, &new_octet, get_taille_fichier(&fd));
            remplissage_fenetre23(&fd, fenetre2, fenetre3, num_bloc);  
            fenetre_active=0;
          }
        }
      break;


      /* Suppression octet */ 
      case TOUCHE_S :
        if(fenetre_active){
          if( (position_souris != -1) || (position_souris != -1) ){
            suppression_octet(&fd, position_souris, nb_octet_tampon, num_bloc, get_taille_fichier(&fd));
            update_mat_tampon();
            remplissage_fenetre23(&fd, fenetre2, fenetre3, num_bloc); 
            message_fenetre1(fenetre1, MESSAGE_6, position_souris, FENETRE_1, num_bloc, &new_octet, get_taille_fichier(&fd)); 
            fenetre_active=0;
          }
        }
      break;
    }      
  }
 
  /* Femeture du fichier ouvert */
  fermeture_fichier(&fd);

  /* Suppresion des fenetres */
  suppresion_fenetre(fenetre1_parent,fenetre1);
  suppresion_fenetre(fenetre2_parent,fenetre2);
  suppresion_fenetre(fenetre3_parent,fenetre3);

  /* Arrêt de ncurses */
  ncurses_stopper();

  return EXIT_SUCCESS;
}

 

 