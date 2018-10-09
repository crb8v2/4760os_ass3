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

#define SHMKEY 123123

void sharedMemory();
void helpMenu();
static void ALARMhandler();


int main(int argc, char **argv) {

    // cc user choice for switch, ss number of processes (def 5),
    // tt is max time until master terminates all children.
    int cc, ss = 5, tt = 2;
    int exampleSize[3];
    char filename [50];

    // BLOCK FOR SHARED MEM
    //shared memory (key, size, permissions)
    int shmid = shmget ( SHMKEY, sizeof(exampleSize[3]), 0775 | IPC_CREAT );
    //get pointer to memory block
    char * paddr = ( char * )( shmat ( shmid, NULL, 0 ) );
    int * pint = ( int * )( paddr );
    pint[0] = 0;
    pint[1] = 0;
    pint[2] = 0;

    //command line options
    while ((cc = getopt (argc, argv, "hl:s:t:")) != -1)
        switch (cc)
        {
            case 'l':
                //save the filename to the logfile
                strncpy(filename, optarg, 50);
                break;
            case 'h':
                helpMenu();
                break;

            case 's':
                ss = atoi(optarg);
                break;

            case 't':
                tt = atoi(optarg);
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
        alarm(tt);

        // fork defaults: 5 forks, terminate after 2 seconds
        // manage
        pid_t pidHolder[ss];
        for(int ii = 0; ii < ss; ii++) {

            if ((pidHolder[ii] = fork()) == 0) {
                execl("./user", "user", NULL);
            }
        }

        for(int aa = 0; aa < 10; aa++){
            pint[0]++;
            printf("%d\n", pint[0]);
            usleep(100000);
        }

        // pid and times to log
        for(int bb = 0; bb < ss; bb++){
            if(wait(NULL)==pidHolder[bb]){
                //create file and write
                printf("pid %d at time %d %d \n", pidHolder[bb], pint[0], pint[1]);
                FILE *fp = fopen(filename,"a+");
                fputs("Child: ", fp);
                fprintf(fp, "%d", pidHolder[bb]);
                fputs(" is terminating at my time ", fp);
                fprintf(fp, "%d %s %d %s", pint[0], "seconds,", pint[1], "nanoseconds.\n");
                fputs(" end \n", fp);
                fclose(fp);
            }
        }

        wait(NULL);
        // clean shared mem
        shmdt(pint);
        printf("\n end of parent \n");

        return 0;
}

void helpMenu() {
    printf("\n -- HELP MENU --\n");
    printf("-h                    open help menu\n");
    printf("-s (int)              specify number of user processes\n");
    printf("-l filename           filename of the log file\n");
    printf("-t (int)              time in seconds master will terminate");
}

// alarm magic
static void ALARMhandler() {
    printf("Time ran out!\n");
    exit(EXIT_SUCCESS);
}












//            FILE *fp = fopen(optarg,"a+");
//            fputs("string bean \n",fp);
//            fputs("string bean \n",fp);
//            fputs("string bean \n",fp);
//            fclose(fp);