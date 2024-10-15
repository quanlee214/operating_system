#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    // Check if the number of arguments is correct
    if (argc != 2) {
        exit(1);
    }

    // Convert the argument to an integer (number of ticks)
    int ticks = atoi(argv[1]);

    // If the conversion fails or the ticks are less than or equal to 0
    if (ticks <= 0) {
        printf("sleep: invalid number of ticks\n");
        exit(1);
    }

    // Call the sleep system call with the given ticks
    sleep(ticks);

    // Exit successfully
    exit(0);
}
