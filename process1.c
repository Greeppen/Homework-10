#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
int main(){
    pid_t children;
    int status;
    printf("Hello world!\n");
    children = fork();
    if (children == 0){
        printf("current child process id: %d\n", getpid());
        printf("parent process id: %d\n", getppid());
        exit(5);
    }
    else{
        printf("current id %d\n", getpid());
        printf("Parent process id %d\n", getppid());
        wait(&status);
        printf("status = %d\n", WEXITSTATUS(status));
    }

    return 0;
}
