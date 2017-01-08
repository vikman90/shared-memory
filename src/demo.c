#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <shm.h>

/******************************************************************************/

static shm_t my_mem = { NULL };

static void *my_alloc(size_t size) {
    void *addr;

    if (my_mem.addr)
        shm_free(&my_mem);

    addr = shm_alloc(&my_mem, size);

    if (!addr) {
        perror("my_alloc");
        exit(EXIT_FAILURE);
    }

    return addr;
}

static void* my_realloc(size_t size) {
    void *addr = my_mem.addr ? shm_realloc(&my_mem, size) : shm_alloc(&my_mem, size);

    if (!addr) {
        perror("my_realloc");
        exit(EXIT_FAILURE);
    }

    return addr;
}

static void print_mem() {
    printf("  Reserved: %*.1f KiB\n" \
           "  Allocated: %*.1f KiB\n", \
           5, my_mem.size / 1024.0, 4, my_mem.length / 1024.0);
}

/******************************************************************************/

int main() {
    char *string;
    pid_t pids[2];

    printf("Default hysteresis: %u page(s).\n\n", shm_get_hysteresis());

    // Allocate memory
    string = my_alloc(2048);
    string[0] = '\0';
    printf("Memory segment created.\n");
    print_mem();

    // Expand segment, no re-mapping
    string = my_realloc(3072);
    printf("Memory segment expanded (little).\n");
    print_mem();

    // Expand segment, with re-mapping
    string = my_realloc(10240);
    printf("Memory segment expanded (big).\n");
    print_mem();

    // Shrink segment, no re-mapping
    string = my_realloc(6144);
    printf("Memory segment shkinked (no re-mapping).\n");
    print_mem();

    // Shrink segment, no re-mapping
    string = my_realloc(2048);
    printf("Memory segment shkinked (re-mapping).\n");
    print_mem();

    printf("\n");

    // Fork into writer child

    switch ((pids[0] = fork())) {
    case -1:
        perror("fork");
        return EXIT_FAILURE;

    case 0:
        printf("Child 1: writing shared message.\n");
        strcpy(string, "Hello world!");
        return EXIT_SUCCESS;
    }

    // Fork into reader child

    switch ((pids[1] = fork())) {
    case -1:
        perror("fork");
        return EXIT_FAILURE;

    case 0:
        sleep(1);
        printf("Child 2: message = '%s'\n", string);
        return EXIT_SUCCESS;
    }

    // Wait for child processes

    waitpid(pids[0], NULL, 0);
    waitpid(pids[1], NULL, 0);

    return EXIT_SUCCESS;
}
