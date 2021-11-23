#ifndef _INTERACTION_
#define _INTERACTION_

#define MESSAGE_1 1
#define MESSAGE_2 2
#define MESSAGE_3 3
#define MESSAGE_4 4
#define MESSAGE_5 5
#define MESSAGE_6 6

#define FENETRE_1 1
#define FENETRE_2 2
#define FENETRE_3 3

#define TOUCHE_ENTRER 10
#define TOUCHE_S 115

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
void message_fenetre1(WINDOW *fenetre1, int indice_message, int position, int indice_fenetre, int num_bloc,unsigned char* new_octet, int taille_fichier);

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
int remplissage_fenetre23(int* fd, WINDOW* fenetre2, WINDOW* fenetre3, int num_bloc);

/**
 * Fonction necessite d'être opitimiser !
 * Determine le numero de la colonne des "00" dans la fenetre 2
 * @param[out] x coordonnée de la colonne lors du clique gauche
 * @return le numero de la colonne
 */
int identification_num_col_f2(int x);

/**
 * Change l'affichage de l'octet sélectionné
 * @param[out] indice_fenetre indice de la fenetre 2 ou 3.
 * @param[out] y coordonnée de hauteur du clique gauche
 * @param[out] x coordonnée de largeur du clique gauche
 * @param[out] indice_fenetre l'indice de la fenetre ( 2 ou 3 )
 */ 
void coloration_indice(WINDOW* fenetre, int y, int x, int indice_fenetre);

/**
 * Lecture du nouvel octet
 * @param[out] new_octet nouvel octet saisie par l'utilisateur
 */
void lecture_saisie(unsigned char* new_octet);

/**
 * Determination si l'octet est affichable ou non
 * Affichage Letres minusucules, majuscules et chiffres  : 
 *  (octet >= 48 && octet <= 57) => Chiffres
 *  (octet >= 65 && octet <= 90) => Majuscules
 *  (octet >= 97 && octet <= 122) => Minuscules
 * @param[out] octet octet représentant le caractère
 */
int est_affichable(unsigned char octet);

#endif