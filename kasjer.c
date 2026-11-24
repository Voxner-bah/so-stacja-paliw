#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

int main(int argc, char *argv[]) {
    int id_kasy = (argc >= 2) ? atoi(argv[1]) : 1;

    printf("[KASJER %d PID=%d] Proces uruchomiony.\n", id_kasy, getpid());
    printf("[KASJER %d] TODO: obsluga kolejki klientow.\n", id_kasy);
    return 0;
}
