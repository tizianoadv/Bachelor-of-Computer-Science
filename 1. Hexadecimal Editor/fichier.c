/**
 * FICHIER
 * Fichier regroupant les fonctions de necessaire à la manipulation des fichiers bas niveau pour ce projet.
 * @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */
#include "fichier.h"
#include "ncurses.h"

#include <sys/types.h>
#include <unistd.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h> 
#include <time.h>

/**
 * Ouverture du fichier en lecture / ecriture
 * Fichier créé s'il n'existe pas
 * Droit Lecture / ecriture utilisateur
 * @param[out] fd le file descriptor du fichier à ouvrir
 * @param[out] nomFichier nom du fichier
 */ 
void ouverture_fichier(int *fd, char* nomFichier){

    if( (*fd = open(nomFichier, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR)) == -1 ){
        ncurses_stopper();
        perror("Erreur open ");
        exit(EXIT_FAILURE);
    }
}

/**
 * Fermeture du fichier ouvert
 * @param[out] fd file descriptor du fichier à fermer
 */
void fermeture_fichier(int *fd){

    if( close(*fd) == -1 ){
        ncurses_stopper();
        perror("Erreur close ");
        exit(EXIT_FAILURE);
    }
}

/**
 * Initialisation du fichier 
 * Ecriture dans le fichier de caractère alphabétiques et numériques choisit aléatoirement
 * Afin d'obtenir une taille aléatoire multiple de 128 et maximum 1024
 * @param[out] fd file descriptor du fichier
 */
void initialisation_fichier(int *fd){
    int i,j;
    int nb_bloc;
    
    srand(time(NULL));

    /* Choix du nombre de bloc : 1 - 8 */
    nb_bloc = (rand() % (8 - 1 + 1 )) + 1;

    /* Remplissage */
    for ( i=0 ; i < nb_bloc ; i++){
        
        /* Caractère choisi aléatoirement */
        for ( j=0 ; j < TAILLE_BLOC ; j++ ){
            TAMPON[j] = (unsigned char) ((rand() % (126 - 33 + 1)) + 33);
        }
        
        /* Ecriture dans le fichier + déplacement tete de lecture */
        if( write(*fd, TAMPON, TAILLE_TAMPON ) == -1 ){
            ncurses_stopper();
            perror("Erreur write ");
            exit(EXIT_FAILURE);
        }
    }
}

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
int lecture_nieme_bloc(int** fd, unsigned char* tampon, int num_bloc){

    int nb_octet_lus;

    /* Deplacement de la tete le lecture vers le numero de bloc choisi */
    if ( (lseek(**fd, num_bloc * TAILLE_BLOC ,SEEK_SET)) == -1 ){
        ncurses_stopper();
        perror("Erreur deplacement fichier ");
        exit(EXIT_FAILURE);
    }

    /* Read max 128 octets dans le fichier et retourne le nombre d'octet lus */
    if ( (nb_octet_lus = read(**fd, tampon, TAILLE_TAMPON)) == -1 ){
        ncurses_stopper();
        perror("Erreur read ");
        exit(EXIT_FAILURE);
    }
    return nb_octet_lus;
}


/**
 * Ecriture et ecrasement de l'ancien bloc par le nouveau issue du TAMPON
 * @param[out] fd le file descriptor du fichier
 * @param[out] num_bloc le numéro du bloc selectionné 
 * @param[out] num_octet le numéro de l'octet selectionné
 * @param[out] nb_octet_tampon le nombre d'octet utile contenu dans le tampon
 */
void ecriture_nieme_bloc(int** fd, int num_bloc, int num_octet,int nb_octet_tampon){

    /* Deplacement de la tete de lecture dans le fichier */
    if ( (lseek(**fd, num_bloc * TAILLE_BLOC ,SEEK_SET)) == -1 ){
        ncurses_stopper();
        perror("Erreur deplacement fichier ");
        exit(EXIT_FAILURE);
    }

    /* Ecriture et ecrasement du bloc contenu dans le tampon vers le même bloc contenu dans le fichier */
    if( ( write(**fd,TAMPON, nb_octet_tampon * sizeof(unsigned char)) ) == -1 ){
        ncurses_stopper();
        perror("Erreur write ");
        exit(EXIT_FAILURE);
    }
}

/**
 * Modification de l'octet selectionné
 * @param[out] fd le file descriptor du fichier
 * @param[out] new_octet L'octet saisie par l'utilisateur
 * @param[out] num_octet Le numéro de l'octet selectionné et à remplacer
 * @param[out] nb_octet_tampon Le nombre d'octet dans le tampon image du nombre d'octet affiché
 * @param[out] num_bloc Le numéro du bloc d'octet
 */
void modification_octet(int* fd, unsigned char new_octet, int num_octet,int nb_octet_tampon, int num_bloc){

    /* Modification */
    *(TAMPON + num_octet ) = new_octet; 

    /* Ecriture et remplacement */
    ecriture_nieme_bloc(&fd, num_bloc, num_octet, nb_octet_tampon);   
}

/**
 * Suppression de l'octet selectionné
 * @param[out] fd le file descriptor du fichier
 * @param[out] num_octet le numero de l'octet à supprimer
 * @param[out] nb_octet_tampon Le nombre d'octet dans le tampon image du nombre d'octet affiché
 * @param[out] num_bloc le numéro du bloc selectionné
 * @param[out] taille_fichier Taille actuelle du fichier
 */
void suppression_octet(int* fd, int num_octet, int nb_octet_tampon,int num_bloc, int taille_fichier){
    int i;

    /* Decalage des octets */
    for( i= (num_octet) ; i < nb_octet_tampon ; i++ ){  *( TAMPON + i )= *( TAMPON + (i+1) ) ; }

    /* Ecriture et écrasement */
    ecriture_nieme_bloc(&fd,num_bloc, num_octet,nb_octet_tampon - 1 ); 

    /* Tronquage du dernier caractère */
    tronquer_fichier(&fd, num_bloc, nb_octet_tampon, taille_fichier);
}

/**
 * Suppresion de l'octet selectionné
 * @param[out] fd le file descriptor du fichier
 * @param[out] num_bloc le numéro du bloc selectionné 
 * @param[out] nb_octet_tampon le nombre d'octet utile contenu dans le tampon
 * @param[out] taille_fichier Taille actuelle du fichier.
 */
void tronquer_fichier(int** fd, int num_bloc, int nb_octet_tampon, int taille_fichier){

    /* Deplacement de la tete le lecture vers l'avant dernier octet du fichier */
    if ( (lseek(**fd, 0L, SEEK_CUR)) == -1 ){
        ncurses_stopper();
        perror("Erreur deplacement fichier ");
        exit(EXIT_FAILURE);
    }

    /* Tronquage */
    if( (ftruncate(**fd, (taille_fichier-1) )) == (off_t)-1 ){
        ncurses_stopper();
        perror("Erreur tronquage : ");
        exit(EXIT_FAILURE);
    }
}

/**
 * Calcule la taille du fichier
 * Fonction necessaire pour connaitre le nombre de "KEY_DOWN" possible
 * @param[out] fd file descriptor du fichier
 * @return la position de la tete de lecture (à la fin de du fichier)
 */
int get_taille_fichier(int* fd){
    return lseek(*fd,0L,SEEK_END);
} 

/**
 * Copie le contenu du TAMPON dans une matrice à deux dimensions
 * Dimension 1 -> 16
 * Dimension 2 -> 8
 */
void update_mat_tampon(){

    int i, j, compteur;

    for ( i=0 , compteur=0 ; i < 16 ; i++ ){
        for ( j=0 ; j < 8 ; j++ , compteur++){

            mat_tampon[i][j] = TAMPON[compteur];
        }
    }
}

/**
 * Transforme les coordonnées du clique gauche 
 * de la fentre 2 en une position
 * @param[out] y coordonée représentant la ligne
 * @param[out] x coordonée représentant la colonne
 * @return la position du clique
 */
int coord_to_position(int y, int x){
    int position_y=0;
    int position_x=0;
    int i, j;

    for( i=0 ; i < y ; i ++, position_y++ ){} 

    for( j=0 ; j < x ; j ++, position_x++ ){}

    return (position_y)*8 + position_x;
}

