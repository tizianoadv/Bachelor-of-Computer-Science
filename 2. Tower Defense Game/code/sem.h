#ifndef _SEM_
#define _SEM_

/* CONSTANTES */
#define CLE_SEM 912

/* STRUCTURES */
/* Structure d'un segment de mémoire" */
typedef struct sem{
    int shmid;
    int cle;
    void *addr;
}sem_t;

/* PROTOTYPES */
/**
 * Création d'un segment de mémoire partagée 
 * @return un segment de mémoire
 */
sem_t creer_sem();
/**
 * Attachement à un segment de mémoire
 * @param[out] sem le segment de memoire à attacher
 */
void attacher_sem(sem_t* sem);
/**
 * Detachement du segment de mémoire
 * @param[out] sem le segment de memoire à détacher 
 */
void detacher_sem(sem_t* sem);
/**
 * Permet de supprimer le segment de mémoire
 * @param[out] sem le segment de memoire à supprimer
 */
void supprimer_sem(sem_t* sem);

#endif