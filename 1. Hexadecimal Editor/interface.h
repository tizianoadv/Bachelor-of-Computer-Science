#ifndef _INTERFACE_
#define _INTERFACE_


/**
 * Dimenssion fenetre 1 parent.
 */
#define LARGEUR_F1P COLS      /* Largeur de la fenêtre 1 parent */
#define HAUTEUR_F1P 7       /* Hauteur de la fenêtre 1 parent */
#define POSX_F1P    0      /* Position horizontale de la fenêtre 1 parent */
#define POSY_F1P    0       /* Position verticale de la fenêtre 1 parent */

/**
 * Dimenssion fenetre 1.
 */
#define LARGEUR_F1 COLS-2      /* Largeur de la fenêtre 1 */
#define HAUTEUR_F1 5        /* Hauteur de la fenêtre 1 */
#define POSX_F1    1      /* Position horizontale de la fenêtre 1 */
#define POSY_F1    1       /* Position verticale de la fenêtre 1 */

/**
 * Dimenssion fenetre 2 parent.
 */
#define LARGEUR_F2P 34      /* Largeur de la fenêtre 2 parent */
#define HAUTEUR_F2P 18       /* Hauteur de la fenêtre 2 parent */
#define POSX_F2P    0      /* Position horizontale de la fenêtre 2 parent */
#define POSY_F2P    7       /* Position verticale de la fenêtre 2 parent */

/**
 * Dimenssion fenetre 2.
 */
#define LARGEUR_F2 32      /* Largeur de la fenêtre */
#define HAUTEUR_F2 16      /* Hauteur de la fenêtre */
#define POSX_F2   1      /* Position horizontale de la fenêtre */
#define POSY_F2   8      /* Position verticale de la fenêtre */

/**
 * Dimenssion fenetre 3 parent.
 */
#define LARGEUR_F3P 10      /* Largeur de la fenêtre 3 parent */
#define HAUTEUR_F3P 18       /* Hauteur de la fenêtre 3 parent */
#define POSX_F3P    35      /* Position horizontale de la fenêtre 3 parent */
#define POSY_F3P    7       /* Position verticale de la fenêtre 3 parent */

/**
 * Dimenssion fenetre 3.
 */
#define LARGEUR_F3 8      /* Largeur de la fenêtre */
#define HAUTEUR_F3 16      /* Hauteur de la fenêtre */
#define POSX_F3   36      /* Position horizontale de la fenêtre */
#define POSY_F3   8       /* Position verticale de la fenêtre */

/**
 * Creation de fenetre parent.
 * @param[out] indice_fenetre l'indice de la fenetre 1, 2 ou 3
 * @return fenetre_parent (WINDOW *)
 */
WINDOW* creation_fenetre_parent(int indice_fenetre);

/**
 * Creation de fenetre (sous fenetre de fenetre parent).
 * @param[out] fenetre_parent fenetre parent de la fenetre à créer
 * @param[out] indice_fenetre l'indice de la fenetre 1, 2 ou 3
 * @return fenetre (WINDOW *)
 */
WINDOW* creation_fenetre(WINDOW* fenetre_parent,int indice);

/**
 * Création et initialisation de l'interface de la fenetre.
 * @param[out] fenetre_parent la fenetre parent
 * @param[out] fenetre la sous fenetre de la fenetre parent
 * @param[out] indice_fenetre l'indice de la fenetre 1, 2 ou 3
 */
void creation_interface(WINDOW *fenetre_parent, WINDOW *fenetre , int indice_fenetre);

/**
 * Supression de fenetre.
 * @param[out] fenetre_parent la fenetre parent
 * @param[out] fenetre la sous fenetre de la fenetre parent
 */
void suppresion_fenetre(WINDOW *fenetre_parent, WINDOW *fenetre);

#endif