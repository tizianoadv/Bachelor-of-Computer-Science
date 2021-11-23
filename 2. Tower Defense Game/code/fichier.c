/**
 * fichier.c
 * Fichier qui repertorie les fonctions de gestion de fichiers de cartes et de scénario
 * Repertoires : 
 *   -> cartes
 *          easy.bin
 *          hard.bin
 *          medium.bin
 *   -> scenarios
 *          infini.bin
 *          pognon.bin
 *          tranquille.bin 
 * @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 * */
#include <stdio.h>    
#include <stdlib.h>
#include <string.h>     /*Pour strcpy memset, memcpy, strcmp, strtok */
#include <sys/stat.h>   /* Pour mkdir */
#include <sys/types.h>  /* Pour S_IRUSR ... (les modes) */ 
#include <errno.h>      /*Pour errno */
#include <fcntl.h>      /* Pour open */
#include <unistd.h>     /* Pour read & lseek */

#include "fichier.h"    /* Pour les prototypes de fonction */
#include "partie.h"     /* Pour les constantes de scenario */

/**
 * Recupère la liste des cartes 
 * @return une liste carte (liste_carte_t)
 **/
liste_carte_t recuperer_liste_carte(){
    int i=0;
    int nb_carte=0;
    liste_carte_t retour;
    DIR* repertoire = NULL;
    struct dirent *entrees = NULL;
    char *nom_fichier = NULL;
    char *extension_fichier = NULL;
    char *fichier_binaire = "bin";
    /* Ouverture du repertoire en lecture */
    if( (repertoire = opendir("cartes")) == NULL ){
        perror("Erreur lors de l'ouverture du repertoire cartes");
        exit(EXIT_FAILURE);
    }
    /* on place la tete de lecture au début du repertoire */
    rewinddir(repertoire);
    while( i < NB_MAX_ENTREES ){
        /* lecture de l'entrée */
        if( (entrees = readdir(repertoire)) != NULL ){
            /* Récupération des noms des fichiers */
            if(entrees->d_name[0] != '.'){
                if((nom_fichier = strtok(entrees->d_name, ".")) != NULL){
                    if( (extension_fichier = strtok(NULL, ".")) != NULL ){
                        if( strcmp(extension_fichier, fichier_binaire) == 0 ){
                            strcpy(retour.cartes[nb_carte].nom, entrees->d_name);
                            nb_carte++;
                        }
                    }
                }
            }
        }     
        i++;
    }
    return retour;
}
/**
 * Recupère la liste des scenarios 
 * @return une liste scenario (liste_scenario_t)
 **/
liste_scenario_t recuperer_liste_scenario(){
    int i=0;
    int nb_carte=0;
    liste_scenario_t retour;
    DIR* repertoire = NULL;
    struct dirent *entrees = NULL;
    char *nom_fichier = NULL;
    char *extension_fichier = NULL;
    char *fichier_binaire = "bin";
    /* Ouverture du repertoire en lecture */
    if( (repertoire = opendir("scenarios")) == NULL ){
        perror("Erreur lors de l'ouverture du repertoire cartes");
        exit(EXIT_FAILURE);
    }
    /* on place la tete de lecture au début du repertoire */
    rewinddir(repertoire);
    while( i < NB_MAX_ENTREES ){
        /* lecture de l'entrée */
        if( (entrees = readdir(repertoire)) != NULL ){
            /* Récupération des noms des fichiers */
            if(entrees->d_name[0] != '.'){
                if((nom_fichier = strtok(entrees->d_name, ".")) != NULL){
                    if( (extension_fichier = strtok(NULL, ".")) != NULL ){
                        if( strcmp(extension_fichier, fichier_binaire) == 0 ){
                            strcpy(retour.scenarios[nb_carte].nom, entrees->d_name);
                            nb_carte++;
                        }
                    }
                }
            }
        }     
        i++;
    }
    return retour;
}
/**
 * Récupération des informations des fichiers binaires des cartes
 * @param[out] nom_carte (char*)
 * @return*chaine (unsigned char*)
 **/
unsigned char* recuperer_infos_carte(char *nom_carte){
    int i;
    int fd=0;
    off_t taille_description=0;
    unsigned char *retour = NULL;
    char pathname[256]="cartes/";

    if( (retour = (unsigned char*) malloc((sizeof(unsigned char)) * 225 )) == NULL){
        perror("Erreur allocation retour recuperation carte");
        exit(EXIT_FAILURE);
    }
    strcat(pathname,nom_carte);
    strcat(pathname,".bin");
    fd = ouvrir_fichier_lecture(pathname);
    tete_lecture_debut(fd);
    lire_taille(fd, &taille_description);
    sauter_description(fd, taille_description);
    for( i=0 ; i < 225 ; i++ )
        lire_case(fd, &retour[i]);
        
    return retour;
}
/**
 * Ouvre un scénario en lecture et place la tete lecture au bon endroit
 * @param[out] nom_scenario (char*)
 * @return fd (int)
 **/
int ouvrir_scenario(char *nom_scenario){
    int fd=0;
    off_t taille_description=0;
    char pathname[256]="scenarios/";

    /* PARTIE PREPARATION SCENARIO */
    /* Ouverture du fichier binaire */ 
    strcat(pathname,nom_scenario);
    strcat(pathname,".bin");
    fd = ouvrir_fichier_lecture(pathname);

    /* Positionnement tete lecture */
    tete_lecture_debut(fd);
    lire_taille(fd, &taille_description);
    sauter_description(fd, taille_description);
    
    return fd;
}
/**
 * Lecture du scenario ligne par ligne
 * @param[out] fd (int)
 * @param[out] total_lu (ssize_t *)
 * @param[out] ligne (ligne_scenario_t *)
 **/
/* Lecture du scenario ligne par ligne */
void lire_scenario(int fd, ssize_t *total_lu, ligne_scenario_t *ligne){  
    ssize_t lu;

    /* Initialisation des variables */
    *total_lu = 0;lu=0;
    memset(ligne,0, sizeof(ligne_scenario_t));

    /*Lecture temps */
    ligne->temps = lire_temps(fd, &lu);
    *total_lu += lu; lu=0;

    /*Lecture type */
    ligne->type  = lire_type(fd, &lu);
    *total_lu += lu; lu=0;

    /* Lectures des données */
    if( *total_lu != 0){
        switch( ligne->type ){
            case 0:
                ligne->msg = lire_chaine(fd, &lu);
            break;
            case 1:
                ligne->unite = lire_data(fd, &lu);
            break;
            case 2:
                ligne->freeze = lire_data(fd, &lu);
            break;
            case 3:
                ligne->argent = lire_data(fd, &lu);
            break;
            default : 
            break;
        }
        *total_lu += lu; lu=0;
    }
}
/**
 * Ouvrir fichier en lecture
 * @param[out] pathname (char *)
 * @return fd (int)
 **/
int ouvrir_fichier_lecture(char *pathname){
    int fd=0;
    if(  (fd = open(pathname, O_RDONLY, S_IRUSR | S_IWUSR  )) == -1 ){
        perror("Erreur dans l'ouverture de la carte");
        exit(EXIT_FAILURE);
    }
    return fd;
}
/**
 * Mise de la tete de lecture au début du fichier
 * @param[out] fd (int)
 **/
void tete_lecture_debut(int fd){
    if ( (lseek(fd, 0L, SEEK_SET)) == (off_t) -1 ){
        perror("Erreur deplacement de la tete de lecture dans la carte");
        exit(EXIT_FAILURE);
    }
}
/**
 * Sauter la description
 * @param[out] fd (int)
 * @param[out] taille_saut (off_t)
 **/
void sauter_description(int fd, off_t taille_saut){
    if ( (lseek(fd, taille_saut, SEEK_CUR)) == (off_t) -1 ){
        perror("Erreur deplacement de la tete de lecture dans la carte");
        exit(EXIT_FAILURE);
    }
}
/**
 * Lecture d'une case
 * @param[out] fd (int)
 * @param[out] buffer (unsigned char *)
 **/
void lire_case(int fd, unsigned char *buffer){
    if ( (read(fd, buffer, sizeof(unsigned char))) == -1 ){
        perror("Erreur read d'une carte lire case");
        exit(EXIT_FAILURE);
    }
}
/**
 * Lecture de la taille de la description
 * @param[out] fd (int)
 * @param[out] taille_description (off_t *)
 **/
void lire_taille(int fd, off_t *taille_description){
    if ( (read(fd, taille_description, sizeof(off_t))) == -1 ){
        perror("Erreur read d'une carte lire taille");
        exit(EXIT_FAILURE);
    }
}
/**
 * Lecture du temps 
 * @param[out] fd (int)
 * @param[out] lu (ssize_t *)
 * @return temp (long)
 **/
long lire_temps(int fd, ssize_t *lu){
    long retour=0;
    if ( (*lu = read(fd, &retour, sizeof(long))) == -1 ){
        perror("Erreur read lire temps");
        exit(EXIT_FAILURE);
    }
    return retour;
}
/**
 * Lecture du type 
 * @param[out] fd (int)
 * @param[out] lu (ssize_t *)
 * @return type (unsigned char)
 **/
unsigned char lire_type(int fd, ssize_t *lu){
    unsigned char retour=0;
    if ( (*lu = read(fd, &retour, sizeof(unsigned char))) == -1 ){
        perror("Erreur read lire type");
        exit(EXIT_FAILURE);
    }
    return retour;
}
/**
 * Lecture d'une chaine 
 * @param[out] fd (int)
 * @param[out] lu (ssize_t *)
 * @return chaine (char*)
 **/
char* lire_chaine(int fd, ssize_t *lu){
    char *retour = NULL;

    if( (retour = (char*) malloc(sizeof(char) *255)) == NULL ){
        perror("Erreur allocation lire chaine");
        exit(EXIT_FAILURE);
    }

    if ( (*lu = read(fd, retour, sizeof(char) * 255 )) == -1 ){
        perror("Erreur read lire chaine");
        exit(EXIT_FAILURE);
    }
    return retour;
}
/**
 * Lecture d'une donnée
 * @param[out] fd (int)
 * @param[out] lu (ssize_t *)
 * @return donnée (unsigned int)
 **/
unsigned int lire_data(int fd, ssize_t *lu){
    unsigned int retour=0;
    if ( (*lu = read(fd, &retour, sizeof(unsigned int))) == -1 ){
        perror("Erreur read lire chaine");
        exit(EXIT_FAILURE);
    }
    return retour;
}