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

    //    printf("\nhello from the user\n");

    int exampleSize[3];

    //random number 1-1000000
    int randomNum = (rand() % (1000000-1)) + 1;

    int shmid = shmget ( SHMKEY, sizeof(exampleSize[3]), 0775 | IPC_CREAT );
    // pointer to shared memory that prints n
    int *cint = ( shmat ( shmid, NULL, 0 ) );

    // create killtime
    int nanos = cint[1];
    int seconds = cint[0];
    int rollover = 0;

    // generates killtime if 1m rolls to 1b = 1sec
    if ((nanos + randomNum) > 999999999){
        rollover = (nanos + randomNum) - 999999999;
        seconds += 1;
        nanos += rollover;
    }

    // *** CREATE SEMAPHORE
//    sem_t *sem = sem_open("/mysem", O_CREAT | O_EXCL, 0777, 0);

    // *** WAIT FOR SEMAPHORE
//    sem_wait(sem);


    // ########################
    // ### CRITICAL SECTION ###
    // ########################

    //waits for time until termination
    if(nanos == cint[1] && seconds == cint[0]){
        cint[2] = getpid();
        shmdt(cint);
        return 0;
    }

    // *** SIGNAL SEMAPHORE ***
//    sem_post(sem);

//    printf("\nhello from the user\n");

 }