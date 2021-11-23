/**
 * sem.c 
 * -> Gestion des fonctions de gestions du segment de mémoire partagée
 * -> Création segment 
 * -> Attachement à un segment de mémoire
 * -> Detachement d'un segment
 * -> Suppression d'un segment
 *  @author Tizano Nardone & Christian RAHAL - Etudiants en L3 Informatique à l'URCA - parcours passerelle - S0605 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>         /* Pour shmget */
#include <sys/shm.h>         /* Pour shmget shmat shmdt */
#include <errno.h>           /* Pour errno */
#include <sys/stat.h>        /* Pour S_IRUSR | S_IWUSR  */
#include <sys/types.h>       /* Pour shmat shmdt*/

#include "sem.h"             /* Pour les prototypes de fonctions de gestion du segment de mémoire  */
#include "partie.h"          /* Pour la structure partie_t et NB_MAX_PARTIE*/

/**
 * Création d'un segment de mémoire partagée 
 * @return un segment de mémoire
 */
sem_t creer_sem(){
    sem_t retour;
    retour.cle = CLE_SEM;
    retour.addr = NULL;
    if( (retour.shmid = shmget(retour.cle, NB_MAX_PARTIE * sizeof(partie_t), S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL)) == -1 ){
        if( errno != EEXIST ){
            perror("Erreur lors de la création du segment\n");
            exit(EXIT_FAILURE);
        }
    }
    return retour;
}

/**
 * Attachement à un segment de mémoire
 * @param[out] sem le segment de memoire à attacher
 */
void attacher_sem(sem_t* sem){
    if( (sem->addr = shmat(sem->shmid, NULL, 0)) == (void*) -1 ){
        perror("Erreur lors de l'attachement au segment de mémoire ");
        exit(EXIT_FAILURE);
    }
}
/**
 * Detachement du segment de mémoire
 * @param[out] sem le segment de memoire à détacher 
 */
void detacher_sem(sem_t* sem){
    if( (shmdt(sem->addr)) == -1 ){
        perror("Erreur lors du detachement au segment de mémoire ");
        exit(EXIT_FAILURE);
    }
}
/**
 * Suppression d'un segment de mémoire
 * @param[out] sem le segment de memoire à supprimer
 */
void supprimer_sem(sem_t* sem){
    if( (shmctl( sem->shmid, IPC_RMID,NULL)) == -1 ){
        perror("Erreur lors de la suppression du segment de mémoire ");
        exit(EXIT_FAILURE);
    }
}