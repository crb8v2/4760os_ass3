#include <stdio.h>
#include <unistd.h>

int main() {

    if (fork() == 0){
        execl("./user", "user", NULL);
    }

    wait(NULL);

    printf("\n end of parent \n");

    return 0;
}