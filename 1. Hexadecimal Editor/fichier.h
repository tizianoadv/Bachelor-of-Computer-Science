#ifndef _FICHIER_
#define _FICHIER_

/* Permet la compilation with no warnings de la fonction ftruncate() */
#ifndef _POSIX_C_SOURCE 
#define _POSIX_C_SOURCE 200112L 
#endif

#define TAILLE_TAMPON 128
#define TAILLE_BLOC 128

unsigned char TAMPON[TAILLE_TAMPON];
unsigned char mat_tampon[16][8];

/**
 * Ouverture du fichier en lecture / ecriture
 * Fichier créé s'il n'existe pas
 * Droit Lecture / ecriture utilisateur
 * @param[out] fd le file descriptor du fichier
 * @param[out] nomFichier nom du fichier
 */ 
void ouverture_fichier(int *fd, char* nomFichier);

/**
 * Fermeture du fichier ouvert
 * @param[out] fd file descriptor du fichier à fermer
 */
void fermeture_fichier(int *fd);

/**
 * Initialisation du fichier 
 * Ecriture dans le fichier de caractère alphabétiques et numériques choisit aléatoirement
 * Afin d'obtenir une taille aléatoire en 128 et 1024
 * @param[out] fd file descriptor du fichier
 */
void initialisation_fichier(int *fd);

/**
 * Lecture du nième bloc de 128 octets
 * @param[out] fd le file descriptor du fichier
 * @param[out] tampon le pointeur contenant le tampon mémoire de 128 octets
 * @param[out] num_bloc le numéro du bloc selectionné 
 * @return nb_octet_lus le nombre d'octet qui a été lus. 
 * On retourne le nombre d'octet lu car cela permet aux fonction d'affichage
 * d'afficher le nombre exacte d'octet lu. Car si read retourne 5 par exemple 
 * que le buffer à une taille de 128, il risque d'afficher plus de caractère que necessaire.
 */
int lecture_nieme_bloc(int** fd, unsigned char* tampon, int num_bloc);

/**
 * Ecriture et ecrasement de l'ancien bloc par le nouveau issue du TAMPON
 * @param[out] fd le file descriptor du fichier
 * @param[out] num_bloc le numéro du bloc selectionné 
 * @param[out] num_octet le numéro de l'octet selectionné
 * @param[out] nb_octet_tampon le nombre d'octet utile contenu dans le tampon
 */
void ecriture_nieme_bloc(int** fd, int num_bloc, int num_octet,int nb_octet_tampon);

/**
 * Modification de l'octet selectionné
 * @param[out] fd le file descriptor du fichier
 * @param[out] new_octet L'octet saisie par l'utilisateur
 * @param[out] num_octet Le numéro de l'octet selectionné et à remplacer
 * @param[out] nb_octet_tampon Le nombre d'octet dans le tampon image du nombre d'octet affiché
 * @param[out] num_bloc Le numéro du bloc d'octet
 */
void modification_octet(int* fd, unsigned char new_octet, int num_octet,int nb_octet_tampon, int num_bloc);

/**
 * Suppression de l'octet selectionné
 * @param[out] fd le file descriptor du fichier
 * @param[out] num_octet le numero de l'octet à supprimer
 * @param[out] nb_octet_tampon Le nombre d'octet dans le tampon image du nombre d'octet affiché
 * @param[out] num_bloc le numéro du bloc selectionné
 * @param[out] taille_fichier Taille actuelle du fichier
 */
void suppression_octet(int* fd, int num_octet, int nb_octet_tampon,int num_bloc, int taille_fichier);

/**
 * Suppresion de l'octet selectionné
 * @param[out] fd le file descriptor du fichier
 * @param[out] num_bloc le numéro du bloc selectionné 
 * @param[out] nb_octet_tampon le nombre d'octet utile contenu dans le tampon
 * @param[out] taille_fichier Taille actuelle du fichier.
 */
void tronquer_fichier(int** fd, int num_bloc, int nb_octet_tampon, int taille_fichier);

/**
 * Calcule la taille du fichier
 * Fonction necessaire pour connaitre le nombre de "KEY_DOWN" possible
 * @param[out] fd file descriptor du fichier
 * @return la position de la tete de lecture (à la fin de du fichier)
 */
int get_taille_fichier(int* fd);

/**
 * Fonction qui copie le contenu du TAMPON dans 
 * une matrice de deux dimensions
 * Dimension 1 -> 16
 * Dimension 2 -> 8
 */
void update_mat_tampon();

/**
 * Transforme les coordonnées du clique gauche en une position
 * @param[out] y coordonée représentant la ligne
 * @param[out] x coordonée représentant la colonne
 * @return la position du clique
 */
int coord_to_position(int y, int x);

#endif