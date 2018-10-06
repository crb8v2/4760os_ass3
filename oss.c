#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>

#define SHMKEY 123123

void sharedMemory();
void helpMenu();
void forker();
static void ALARMhandler();


int main(int argc, char **argv) {

    // cc user choice for switch, ss number of processes (def 5),
    // tt is max time until master terminates all children.
    int cc, ss = 5, tt = 2;
    int exampleSize[2];

    // BLOCK FOR SHARED MEM
    //shared memory (key, size, permissions)
    int shmid = shmget ( SHMKEY, sizeof(exampleSize[2]), 0775 | IPC_CREAT );
    //get pointer to memory block
    char * paddr = ( char * )( shmat ( shmid, NULL, 0 ) );
    int * pint = ( int * )( paddr );
    //checks shmem
//    pint[0] = 666;
//    printf("%d\n", pint[1]);

    //command line options
    while ((cc = getopt (argc, argv, "hs:n:")) != -1)
        switch (cc)
        {
            case 'h':
                helpMenu();
                break;

            case 's':
                ss = atoi(optarg);           // forks allowed at once
                break;

            case 't':
                tt = atoi(optarg);           // master max time
                break;

            case '?':
                if (optopt == 'n')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
                return 1;

            default:
                abort();
        }

        //alarm times out if forks all do not return in 2 seconds
        signal(SIGALRM, ALARMhandler);
        alarm(2);

        // fork defaults: 5 forks, terminate after 2 seconds
        for(int ii = 0; ii < ss; ii++) {
            forker();
        }

        // clean shared mem
        shmdt(pint);
        printf("\n end of parent \n");

        return 0;
}

void forker() {

    if (fork() == 0){
        execl("./user", "user", NULL);
    }

    wait(NULL);
}

void helpMenu() {
    printf("\n -- HELP MENU --\n");
    printf("-h                    open help menu\n");
    printf("-s (int)              specify number of user processes\n");
    printf("-l filename           filename of the log file\n");
    printf("-t (int)              time in seconds master will terminate");
}

// alarm magic
static void ALARMhandler()
{
    printf("Time ran out!\n");
    exit(EXIT_SUCCESS);
}