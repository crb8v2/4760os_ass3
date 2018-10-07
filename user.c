
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMKEY 123123

int main () {

    int exampleSize[3];

    int shmid = shmget ( SHMKEY, sizeof(exampleSize[3]), 0775 | IPC_CREAT );
    // pointer to shared memory that prints n
    int *cint = ( shmat ( shmid, NULL, 0 ) );
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

    printf("\nhello from the user\n");

    usleep(500000);

    return 0;
}


