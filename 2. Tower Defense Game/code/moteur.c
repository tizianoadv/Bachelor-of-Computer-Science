/**
 * moteur.c 
 * -> Gestion du moteur du jeu 
 *  -> gestion de scénario
 *  -> gestion d'ajout & de déplacement des unités
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>             /* Pour memeset */
#include <unistd.h>             /* Pour sleep  et ssize_t */

#include "moteur.h"             /* Pour les prototypes */
#include "fenetre.h"            /* Pour les types fenetres de interface.h */
#include "interface.h"          /* Pour les CASE */
#include "fichier.h"            /* Pour la fonction lire_scenario() */

/**
 * Modification des informations de la partie en fonctione du scénario
 * @param[out] socket_communication (socket_t *)
 * @param[out] partie (partie_t *)
 * @param[out] jeu_joueur (jeu_t *)
 **/
void gerer_scenario(socket_t *socket_communication, partie_t *partie, jeu_t *jeu_joueur){
    int CASE_ORDI_J, CASE_ORDI_I;
    int fd_scenario;
    ligne_scenario_t scenario_l;
    requete_tcp_t requete_envoie;
    ssize_t total_lu=-1;

    /* PREPARATIION DEBUT SCENARIO */
    /* Ouverture et préparation du scénario */
    fd_scenario = ouvrir_scenario(partie->scenario.nom);
    /*Récuperaition de la case de l'ordi */
    recuperer_case_ordi(jeu_joueur, &CASE_ORDI_I, &CASE_ORDI_J);

    /*TANT QU'ON A PAS TOUT LU LE SCENARIO 
    OU QUE LE JOUEUR A ENCORE DE LA VIE*/
    while(total_lu != 0 && jeu_joueur->vies > 0 ){

        /* LECTURE DU SCENARIO */
        memset(&scenario_l, 0, sizeof(ligne_scenario_t));
        lire_scenario(fd_scenario, &total_lu, &scenario_l);

        /* MISE A JOUR DES INFORMATIONS */
        if(total_lu != 0){
            switch(scenario_l.type){
                case 0:
                    /*lecture du msg*/
                break;
                /*Si le type est une unité*/
                case 1 :
                    /*Attente du temps d'attente*/
                    sleep((scenario_l.temps)/1000);

                    /*Ajout de la nouvelle unité & envoie requete */
                    ajouter_unite(jeu_joueur, &scenario_l, CASE_ORDI_I, CASE_ORDI_J);
                    preparer_requete_jeu(partie, jeu_joueur, &scenario_l, &requete_envoie);
                    envoyer_requete_tcp(socket_communication, &requete_envoie);
                    sleep(1);

                    /*Déplacement des unités & envoie requete */
                    deplacer_unite(jeu_joueur, CASE_SOLDAT);
                    deplacer_unite(jeu_joueur, CASE_COMMANDO);
                    deplacer_unite(jeu_joueur, CASE_VEHICULE);
                    deplacer_unite(jeu_joueur, CASE_MISSILE);
                    deplacer_unite(jeu_joueur, CASE_CHAR);
                    preparer_requete_jeu(partie, jeu_joueur, &scenario_l, &requete_envoie);
                    envoyer_requete_tcp(socket_communication, &requete_envoie);

                    /*Déplacement des unités pour preparer que
                    les unités ont une case qui les séparent */
                    deplacer_unite(jeu_joueur, CASE_SOLDAT);
                    deplacer_unite(jeu_joueur, CASE_COMMANDO);
                    deplacer_unite(jeu_joueur, CASE_VEHICULE);
                    deplacer_unite(jeu_joueur, CASE_MISSILE);
                    deplacer_unite(jeu_joueur, CASE_CHAR);
                    break;
                
                case 2 :
                    /* freeze */
                    break;

                case 3 :
                    /* argent */
                    break;

                default : 
                    break;
            }
        }
    }
    /* Joueur courant a perdu 
    -> diminution du nombre de joueur de la partie*/
    (partie->nb_joueur_courant)--; 
}

/**
 *  Recuperation la case de l'ordi
 * @param[out] jeu (jeu_t *)
 * @param[out] CASE_ORDI_I (int *)
 * @param[out] CASE_ORDI_J (int *)
 **/
void recuperer_case_ordi(jeu_t *jeu, int *CASE_ORDI_I, int *CASE_ORDI_J){
    int stop_recherche=0;
    int i=0,j=0;
    for(i=0; i<15 && stop_recherche==0  ;i++){
        for(j=0; j<15 && stop_recherche==0 ;j++){
            if( jeu->carte[i][j] == CASE_ORDI ){
                stop_recherche++;
            }
                
        }
    }
    i--;j--;
    *(CASE_ORDI_I) = i;
    *(CASE_ORDI_J) = j;
}

/**
 * Ajout nouvel unité 
 * @param[out] jeu (jeu_t *)
 * @param[out] scenario (ligne_scenario_t *)
 * @param[out] CASE_ORDI_I (int *)
 * @param[out] CASE_ORDI_J (int *)
 **/
void ajouter_unite(jeu_t *jeu, ligne_scenario_t *scenario, int CASE_ORDI_I, int CASE_ORDI_J){
    int i,j;

    i=CASE_ORDI_I; j=CASE_ORDI_J;
    switch(scenario->unite){
        case 1:
            jeu->carte[i][j] = CASE_SOLDAT;
        break;
        case 2:
            jeu->carte[i][j] = CASE_COMMANDO;
        break;
        case 3:
            jeu->carte[i][j] = CASE_VEHICULE;
        break;
        case 4:
            jeu->carte[i][j] = CASE_MISSILE;
        break;
        case 5:
            jeu->carte[i][j] = CASE_CHAR;
        break;
    }
}

/**
 * Vérification des position d'une unité dans la matrice
 * @param[out] jeu (jeu_t *)
 * @param[out] i (int)
 * @param[out] ij(int)
 * @param[out] tab (unsigned char*)
 **/
void verifier_position(jeu_t *jeu, int i, int j, unsigned char* tab){
    /*Initialisation*/
    tab[0]=0;tab[1]=0;tab[2]=0;tab[3]=0;
    /* Position dans un coin */
    if( i==0 && j==0){
        tab[0] = jeu->carte[i][j+1];
        tab[1] = jeu->carte[i+1][j];
    }else if( i==0  && j==14){
        tab[0] = jeu->carte[i][j-1];
        tab[1] = jeu->carte[i+1][j];
    }else if( i==14  && j==0 ){
        tab[0] = jeu->carte[i-1][j];
        tab[1] = jeu->carte[i][j+1];  
    }else if( i==14  && j==14 ){
        tab[0] = jeu->carte[i-1][j];
        tab[1] = jeu->carte[i][j-1];  
    }
    /* Position sur un bord*/
    else if( i==0 && j!=0 && j!=14 ){
        tab[0] = jeu->carte[i][j+1];
        tab[1] = jeu->carte[i+1][j];
        tab[2] = jeu->carte[i][j-1];
    }else if( i==14 && j!=0 && j!=14  ){
        tab[0] = jeu->carte[i-1][j];
        tab[1] = jeu->carte[i][j+1];
        tab[2] = jeu->carte[i][j-1];
    }else if( j==0 && i!=0 && i!=14 ){
        tab[0] = jeu->carte[i-1][j];
        tab[1] = jeu->carte[i][j+1];
        tab[2] = jeu->carte[i+1][j];
    }else if( j==14 && i!=0 && i!=14){
        tab[0] = jeu->carte[i-1][j];
        tab[1] = jeu->carte[i+1][j];
        tab[2] = jeu->carte[i][j-1];
    /*Autres positions */
    }else{
        tab[0] = jeu->carte[i-1][j];
        tab[1] = jeu->carte[i][j+1];
        tab[2] = jeu->carte[i+1][j];
        tab[3] = jeu->carte[i][j-1];
    }
} 

/**
 * Déplacement des unités présentes sur la matrice
 * @param[out] jeu (jeu_t *)
 * @param[out] unite (int)
 **/
void deplacer_unite(jeu_t *jeu, int unite){
    int i=0,j=0,k=0;
    int tmp=0;
    int taille_tab=4;
    int max=0;
    int test = 0;
    int ind[100] = {0};
    int l = 0;
    unsigned char case_adj[4];
    memset(case_adj,0,sizeof(unsigned char)*4);
    /* Déplacement des unités */
    i = 0;
    /* On parcourt la matrice */
    while(i < 15){
        j = 0;
        while( j < 15 ){
            test = 0;
            /* On regarde si j et i sont dans le tableau ind et s'ils se suivent */
            /* Si c'est le cas, alors on passe cette case car sinon les unités avanceront de plusieurs cases */
            if(chercher(ind, j, i) == 1){
                j++;
            }
            if( jeu->carte[i][j] == unite ){
                /* Vérification des position dans matrice */
                verifier_position(jeu,i,j,case_adj);
                /* Recherche valeur max */
                taille_tab = 4;
                /* On tri le tableau case_adj pour déterminer la valeur max */
                /* tri à bulle */
                while( taille_tab > 1 ){ 
                    for( k=0 ; k <  taille_tab -1 ; k++ ){
                        if( case_adj[k] > case_adj[k+1] ){ 
                            tmp = case_adj[k];
                            case_adj[k] = case_adj[k+1];
                            case_adj[k+1] = tmp;
                        }
                    }
                    taille_tab--;
                }
                /* Correspond à la valeur maximum aux alentours de l'unité */
                max = case_adj[3];
                /* Si le max est une unité ou CASE ORDI -> case_max == avant derniere case */
                if( (max >= CASE_SOLDAT && max <= CASE_CHAR) || max == CASE_ORDI ) {
                    max=case_adj[2];
                }
                if( max != CASE_FORT ){
                    /* Déplacement de l'unité sur une nouvelle case en haut */
                    if( max == jeu->carte[i-1][j] ){
                        jeu->carte[i-1][j] = unite;
                    }
                    /* Déplacement de l'unité sur une nouvelle case à droite */
                    else if( max == jeu->carte[i][j+1] ){
                        jeu->carte[i][j+1] = unite; 
                        test++;
                    }
                    /* Déplacement de l'unité sur une nouvelle case en bas */
                    else if( max == jeu->carte[i+1][j] ){
                        jeu->carte[i+1][j] = unite;
                        /* On ajoute les positions de la case ou l'unité s'est déplacée */
                        ind[l] = i+1;
                        ind[l+1] = j;
                        l+= 2; 
                    }
                    /* Déplacement de l'unité sur une nouvelle case à gauche */
                    else {
                        jeu->carte[i][j-1] = unite;
                    }   
                    /* Mise à jour de la valeur de la case précente*/
                    jeu->carte[i][j] = max - 1;
                    /* Si la valeur de la case précédente vaut 4 */
                    if( jeu->carte[i][j] == 4 ){
                        /* Si on est sur une arete ou le sommet de la carte */
                        if( i==14 || j==14 )
                            jeu->carte[i][j] = CASE_ORDI;
                        else
                            jeu->carte[i][j] = CASE_MIN_CHEMIN;
                    }
                }
                else{
                    /* Si l'unité atteint le fort, on décrémente la vie du fort */
                    jeu->carte[i][j] = CASE_MAX_CHEMIN;
                    (jeu->vies)--;
                }
            }
            /* Si l'unité s'est déplacé vers la droite, 
               on augmente j de 2 pour ne pas retomber sur la meme unité 
               lors de la prochaine incrémentation de j car j va de gauche à droite */
            if(test != 0){
                j+=2;
            }
            else{
                j++;
            }
        }
        i++;
    }
}


/**
 * Permet de chercher si les entiers x et y sont dans le tableau pos et s'ils se suivent
 * @param[out] x La position selon l'axe des ordonnées
 * @param[out] y La position selon l'axe des abscisses
 * @return 1 si les valeurs sont dans le tableau et 0 sinon
 */ 
int chercher(int pos[100], int x, int y){
    int i;
    int retour = 0;
    for(i = 0; i < 100; i+=2){
        /* On regarde si la valeur de y et x sont dans le tableau et s'ils se suivent */
        if((pos[i] == y) && (pos[i+1] == x)){
            retour = 1;
        }
    }
    return retour;
}

/**
 * Prépare la requete pour l'envoie du jeu du jour 
 * @param[out] partie (partie_t *)
 * @param[out] jeu (jeu_t *)
 * @param[out] scenario_l (ligne_scenario_t *)
 * @param[out] requete_envoie (requete_tcp_t *)
 **/
void preparer_requete_jeu(partie_t *partie, jeu_t *jeu, ligne_scenario_t *scenario_l, requete_tcp_t *requete_envoie){
    memset(requete_envoie, 0, sizeof(requete_tcp_t));
    requete_envoie->id = UPDATE_JEU;
    requete_envoie->id_partie = partie->id;
    memcpy(&requete_envoie->ligne_scenario, scenario_l, sizeof(ligne_scenario_t));
    memcpy(&requete_envoie->scenario, &partie->scenario, sizeof(scenario_t));
    memcpy(&requete_envoie->jeu, jeu, sizeof(jeu_t));
    memcpy(&requete_envoie->carte, &partie->carte, sizeof(carte_t));
}



