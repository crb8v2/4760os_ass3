
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMKEY 123123

int main () {

    int exampleSize[2];

    int shmid = shmget ( SHMKEY, sizeof(exampleSize[2]), 0775 | IPC_CREAT );
    // pointer to shared memory that prints n
    int *cint = ( shmat ( shmid, NULL, 0 ) );
    //shmem tests
//    cint[1] = 777;
//    printf("%d\n", cint[0]);


    printf("\nhello from the OS\n");

    usleep(500000);

    return 0;
}


