#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>

#define LICZBA_DYSTRYBUTOROW 8
#define LICZBA_LADOWAREK 2
#define LICZBA_STANOWISK (LICZBA_DYSTRYBUTOROW + LICZBA_LADOWAREK)
#define MIEJSCA_PARKINGOWE 10
#define LICZBA_KAS 3
#define MAX_KIEROWCOW_DOMYSLNIE 100

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

#endif