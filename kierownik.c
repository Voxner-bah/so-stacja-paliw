#include <stdio.h>
#include <unistd.h>
#include "common.h"

int main(void) {
    printf("[KIEROWNIK PID=%d] Proces uruchomiony.\n", getpid());
    printf("[KIEROWNIK] TODO: obsluga sterowania i wysylanie sygnalow.\n");
    return 0;
}
