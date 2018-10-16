#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHMKEY 123123

int flag = 0;

void sharedMemory();
void helpMenu();
static void ALARMhandler();

int main(int argc, char **argv) {

    // cc user choice for switch, ss number of processes (def 5),
    // tt is max time until master terminates all children.
    int cc, ss = 5, tt = 2;
    int exampleSize[3];
    char filename [50];
    int forkCount = 0;


    //#############################
    // ### BLOCK FOR SHARED MEM ###
    //#############################
    //shared memory (key, size, permissions)
    int shmid = shmget ( SHMKEY, sizeof(exampleSize[3]), 0775 | IPC_CREAT );
    //get pointer to memory block
    char * paddr = ( char * )( shmat ( shmid, NULL, 0 ) );
    int * pint = ( int * )( paddr );
    // 0: seconds 1: nanoseconds 2: shmsg
    pint[0] = 0;
    pint[1] = 0;
    pint[2] = 0;

    // *** CREATE SEMAPHORE
//    sem_t *sem = sem_open("/mysem", O_CREAT | O_EXCL, 0777, 0);

    //command line options
    while ((cc = getopt (argc, argv, "hl:s:t:")) != -1)
        switch (cc)
        {

            case 'l': // for filename
                //save the filename to the logfile
                strncpy(filename, optarg, 50);
                break;
            case 'h': //help menu
                helpMenu();
                return 0;

            case 's': // number of active children
                ss = atoi(optarg);
                break;

            case 't': // time to terminate
                tt = atoi(optarg);
                break;

            case '?': // error handles
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
        // clean shared mem with alarm
        alarm(tt);

        // fork defaults: 5 forks, terminate after 2 seconds
        // pidHolder[] holds the pid's of active children
        forkCount = ss;
        pid_t pidHolder[ss];
        for(int ii = 0; ii < ss; ii++) {

            if ((pidHolder[ii] = fork()) == 0) {
                execl("./user", "user", NULL);
            }
        }

        // system clock: 1,000,000 cycles
        while(1 && flag == 0){
            pint[1] += 20000;

            //check forks
            if(forkCount == 100){
                printf("Terminated after 100 forks\n");

                // parent waits for all children to finish first
                wait(NULL);
                // clean shared mem
                shmdt(pint);

                return 0;
            }



            // adjust second and nanos
            if(pint[1] > 999999999){
                pint[0]++;
                pint[1] = 0;
            }

            printf("%d\n", pint[0]);


            // checks shMsh, pint[2], for a flag that child has completed
            if(pint[2] > 0){
                // pid and times to log

                // *** WAIT FOR SEMAPHORE
//              sem_wait(sem);

                for(int bb = 0; bb < ss; bb++){
                    if(pint[2] == pidHolder[bb]) {
                        //#########################
                        //### WRITE TO LOG FILE ###
                        //#########################
                        //create file and write
                        FILE *fp = fopen(filename, "a+");
                        fputs("Child: ", fp);
                        fprintf(fp, "%d", pidHolder[bb]);
                        fputs(" is terminating at my time ", fp);
                        fprintf(fp, "%d %s %d %s", pint[0], "seconds,", pint[1], "nanoseconds.\n");
                        fclose(fp);

                        forkCount++;

                        // replace pidholder with new fork pid and exec
                        if ((pidHolder[bb] = fork()) == 0)
                            execl("./user", "user", NULL);

                    }
                }

                //flag back to 0
                pint[2] = 0;

                // *** SIGNAL SEMAPHORE ***
//              sem_post(sem);
            }
        }

        // parent waits for all children to finish first
        wait(NULL);
        // clean shared mem
        shmdt(pint);


        // clear shared memory
//        sem_unlink(sem);

        printf("\n end of parent \n");
//        sig_usr(1);

        return 0;
}

void helpMenu() {
    printf("\n -- HELP MENU --\n");
    printf("-h                    open help menu\n");
    printf("-s (int)              specify number of user processes\n");
    printf("-l filename           filename of the log file\n");
    printf("-t (int)              time in seconds master will terminate\n");
}

// alarm magic
static void ALARMhandler() {
    printf("Time ran out!\n");
    flag = 1;
    exit(EXIT_SUCCESS);
}
