#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>


#define SHMKEY 123123

int criticalSection();

sem_t *mutex;

int main () {

    int exampleSize[3];
    int randomNum = rand() % 1000000;

    int shmid = shmget ( SHMKEY, sizeof(exampleSize[3]), 0775 | IPC_CREAT );
    // pointer to shared memory that prints n
    int *cint = ( shmat ( shmid, NULL, 0 ) );

    // create killtime


    printf("\nhello from the user\n");

    // *** CREATE SEMAPHORE
//    sem_t *sem = sem_open("mysem", O_CREAT | O_EXCL, 0777, 1);


    // *** WAIT FOR SEMAPHORE
//    sem_wait(sem);


    // ########################
    // ### CRITICAL SECTION ###
    // ########################
    cint[2] = getpid();

    // *** SIGNAL SEMAPHORE ***
//    sem_post(sem);


// semunlink with "mysem"
//    usleep(500000);


    return 0;
}