
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

    int shmid = shmget ( SHMKEY, sizeof(exampleSize[3]), 0775 | IPC_CREAT );
    // pointer to shared memory that prints n
    int *cint = ( shmat ( shmid, NULL, 0 ) );

//    sem_t *sem = sem_open("mysem", O_CREAT | O_EXCL, 0777, 1);
//
//    sem_wait(mutex);

    cint[2] = 1;

//    sem_post(mutex);

    usleep(500000);
    printf("\nhello from the user\n");

    return 0;
}


//    sem_t mutex;
//
//    wait(mutex);

//    critcal section

// signal()

int criticalSection() {


    printf("hello from the critical section");
    return 1;
}




















//shmem tests
//    cint[1] = 777;
//    printf("%d\n", cint[0]);

//TO DO
// read clock time
// generate rand number 1-1,000,000
// add to clock time and create termination deadline
//
// critical section with semaphors
//  check if termination time
//  cede critical section to another child


