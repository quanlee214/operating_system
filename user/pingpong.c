#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    char* a[2] = {"a",0};

    if (fork() == 0) {
        close(p1[1]);
        read(p1[0],a,2);

        printf("%d: received ping\n", getpid());

        close(p2[0]);
        write(p2[1],a,1);
        close(p2[1]);
    } else {
        close(p1[0]);
       
        write(p1[1],a,1);
        close(p1[1]);
       
        close(p2[1]);
        read(p2[0],a,2);
        printf("%d: received pong\n", getpid());
        close(p2[0]);
    }
    exit(0);
}
