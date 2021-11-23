#ifndef _JEU_
#define _JEU_

/**
 * Ce fichier permet de tester l'interface en lui fournissant les données nécessaires.
 * Pour le projet, vous pouvez/devez le faire différemment...
 */

/* La structure contenant les informations sur le jeu */
typedef struct {
    unsigned char carte[15][15];   /* La carte */
    unsigned int vies;             /* Vies du joueur */
    unsigned int adv[3];           /* Vies des adversaires */
    unsigned int argent;           /* Argent */
    unsigned int freeze;           /* Etat freeze */
    unsigned int unfreeze;         /* Etat unfreeze */
} jeu_t;

/* Constantes pour les coûts des unités */
#define SOLDAT_COUT       100
#define COMMANDO_COUT     300
#define VEHICULE_COUT     700
#define MISSILE_COUT     1500
#define CHAR_COUT        3100

/* Constantes pour les vies des unités */
#define SOLDAT_VIE          5
#define COMMANDO_VIE       10
#define VEHICULE_VIE       20
#define MISSILE_VIE        50
#define CHAR_VIE          100

/* Constantes pour les déplacements des unités */
#define SOLDAT_VITESSE   1000
#define COMMANDO_VITESSE  900
#define VEHICULE_VITESSE  500
#define MISSILE_VITESSE   300
#define CHAR_VITESSE      400

/* Constantes pour les coûts des tourelles */
#define TOUR_1_COUT       100
#define TOUR_2_COUT       300
#define TOUR_3_COUT       700
#define TOUR_4_COUT      1500
#define TOUR_5_COUT      3100

/* Constantes pour les dégâts des tirs des tourelles */
#define TOUR_1_TIR_MIN      1
#define TOUR_1_TIR_MAX     10
#define TOUR_1_PORTEE       1
#define TOUR_1_VITESSE    500

#define TOUR_2_TIR_MIN      5
#define TOUR_2_TIR_MAX     20
#define TOUR_2_PORTEE       2
#define TOUR_2_VITESSE    400

#define TOUR_3_TIR_MIN     10
#define TOUR_3_TIR_MAX     40
#define TOUR_3_PORTEE       3
#define TOUR_3_VITESSE    300

#define TOUR_4_TIR_MIN     20
#define TOUR_4_TIR_MAX    100
#define TOUR_4_PORTEE       4
#define TOUR_4_VITESSE    200

#define TOUR_5_TIR_MIN     50
#define TOUR_5_TIR_MAX    200
#define TOUR_5_PORTEE       5
#define TOUR_5_VITESSE    100

#define FREEZE_DUREE     5000

#endif