#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    pid_t child_pid1, child_pid2;
    int status1, status2;
    printf("Hello! \n");
    child_pid1 = fork();
    if (child_pid1 == 0){
        printf("current child1 process id: %d\n", getpid());
        printf("parent1 process id: %d\n", getppid());
        child_pid1 = fork();
        if (child_pid1 == 0){
            printf("child11 ppid: %d\n", getpid());
            printf("parent11 ppid: %d\n", getppid());
            exit(5);
        }
        child_pid2 = fork();
        if (child_pid2 == 0){
            printf("child12 ppid: %d\n", getpid());
            printf("parent12 ppid: %d\n", getppid());
            exit(5);
        }
        wait(NULL);
        exit(5);
    }
    child_pid2 = fork();
    if (child_pid2 == 0){
        printf("current child2 process id: %d\n", getpid());
        printf("parent2 process id: %d\n", getppid());
        child_pid1 = fork();
        if (child_pid1 == 0){
            printf("child21 ppid: %d\n", getpid());
            printf("parent21 ppid %d\n", getppid());
            exit(5);
        }
        wait(NULL);
        exit(5);
    } 
    else{
        printf("current id: %d\n",getpid());
        printf("Parent id: %d\n", getppid());
        wait(&status1);
        wait(&status2);
        printf("status = %d\n", WEXITSTATUS(status1));
        printf("status = %d\n", WEXITSTATUS(status2));
    }
    return 0;
}
