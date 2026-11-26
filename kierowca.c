#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "common.h"

/* Proste losowanie danych kierowcy do testow */
static void losuj_kierowce(Kierowca *k) {
    k->pid = getpid();
    k->typ_pojazdu = (TypPojazdu)(rand() % 4);
    k->paliwo = (RodzajPaliwa)(rand() % 9);
    k->vip = (rand() % 1000 < 3) ? 1 : 0;
    k->bistro = (rand() % 100 < 20) ? 1 : 0;
    k->tylko_zakupy = (rand() % 100 < 2) ? 1 : 0;
    k->zjazd_przed_zaplata = (rand() % 100 < 2) ? 1 : 0;
}

int main(void) {
    Kierowca k;

    srand((unsigned int)(time(NULL) ^ getpid()));
    losuj_kierowce(&k);

    printf("[KIEROWCA PID=%d] typ=%d paliwo=%d vip=%d bistro=%d zakupy=%d\n",
           k.pid, k.typ_pojazdu, k.paliwo, k.vip, k.bistro, k.tylko_zakupy);
    printf("[KIEROWCA PID=%d] TODO: wybor stanowiska, tankowanie i platnosc.\n", k.pid);
    return 0;
}
