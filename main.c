#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Glowna struktura stanu stacji */
typedef struct {
    Stanowisko stanowiska[LICZBA_STANOWISK];
    Kasa kasy[LICZBA_KAS];
    int zajete_miejsca_parkingowe;
    int aktywni_kierowcy;
    int max_kierowcow;
    int czynne_kasy;
    int godzina_symulacji;
    int skala_czasu_ms;
    int stacja_otwarta;
    int alarm_pozarowy;
} StanStacji;

/* Ustawienie poczatkowego stanu stacji */
static void inicjalizuj_stacje(StanStacji *stacja, int max_kierowcow, int skala_ms) {
    int i;

    for (i = 0; i < LICZBA_STANOWISK; i++) {
        stacja->stanowiska[i].id = i + 1;
        stacja->stanowiska[i].otwarte = 1;
        stacja->stanowiska[i].zajete = 0;
        stacja->stanowiska[i].licznik_zero = 1;
        stacja->stanowiska[i].dlugosc_kolejki = 0;
        stacja->stanowiska[i].obecny_kierowca = 0;
    }

    for (i = 0; i < LICZBA_KAS; i++) {
        stacja->kasy[i].id = i + 1;
        stacja->kasy[i].strefa_bistro = (i == 0) ? 1 : 0;
        stacja->kasy[i].czynna = (i < 2) ? 1 : 0;
        stacja->kasy[i].dlugosc_kolejki = 0;
        stacja->kasy[i].pid_kasjera = 0;
    }

    stacja->zajete_miejsca_parkingowe = 0;
    stacja->aktywni_kierowcy = 0;
    stacja->max_kierowcow = max_kierowcow;
    stacja->czynne_kasy = 2;
    stacja->godzina_symulacji = 9;
    stacja->skala_czasu_ms = skala_ms;
    stacja->stacja_otwarta = 1;
    stacja->alarm_pozarowy = 0;
}

static void wypisz_stan(const StanStacji *stacja) {
    printf("[MAIN PID=%d] Start symulacji stacji paliw\n", getpid());
    printf("Stanowiska: %d (dystrybutory: %d, ladowarki: %d)\n",
           LICZBA_STANOWISK, LICZBA_DYSTRYBUTOROW, LICZBA_LADOWAREK);
    printf("Parking: %d/%d | Czynne kasy: %d/%d | Godzina: %02d:00\n",
           stacja->zajete_miejsca_parkingowe, MIEJSCA_PARKINGOWE,
           stacja->czynne_kasy, LICZBA_KAS, stacja->godzina_symulacji);
    printf("Limit kierowcow: %d | Skala czasu: %d ms\n",
           stacja->max_kierowcow, stacja->skala_czasu_ms);
}

int main(int argc, char *argv[]) {
    int max_kierowcow = MAX_KIEROWCOW_DOMYSLNIE;
    int skala_ms = 100;
    StanStacji stacja;


    return 0;
}
