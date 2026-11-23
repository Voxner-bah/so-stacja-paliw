#ifndef COMMON_H
#define COMMON_H

#include <sys/types.h>

#define LICZBA_DYSTRYBUTOROW 8
#define LICZBA_LADOWAREK 2
#define LICZBA_STANOWISK (LICZBA_DYSTRYBUTOROW + LICZBA_LADOWAREK)
#define MIEJSCA_PARKINGOWE 10
#define LICZBA_KAS 3
#define MAX_KIEROWCOW_DOMYSLNIE 100

/* Rodzaje paliw i napedow */
typedef enum {
    PALIWO_BRAK = 0,
    PALIWO_E5,
    PALIWO_E10,
    PALIWO_B7,
    PALIWO_B10,
    PALIWO_LPG,
    PALIWO_CNG_LNG,
    PALIWO_H2,
    PALIWO_EV
} RodzajPaliwa;

/* Typy pojazdow wjezdzajacych na stacje */
typedef enum {
    POJAZD_OSOBOWY = 0,
    POJAZD_CIEZAROWY,
    POJAZD_AUTOBUS,
    POJAZD_ELEKTRYCZNY
} TypPojazdu;

/* Stan pojedynczego dystrybutora lub ladowarki */
typedef struct {
    int id;
    int otwarte;
    int zajete;
    int licznik_zero;
    int dlugosc_kolejki;
    pid_t obecny_kierowca;
} Stanowisko;

/* Stan pojedynczej kasy w pawilonie */
typedef struct {
    int id;
    int czynna;
    int strefa_bistro;
    int dlugosc_kolejki;
    pid_t pid_kasjera;
} Kasa;

/* Dane pojedynczego kierowcy */
typedef struct {
    pid_t pid;
    TypPojazdu typ_pojazdu;
    RodzajPaliwa paliwo;
    int vip;
    int bistro;
    int tylko_zakupy;
    int zjazd_przed_zaplata;
} Kierowca;

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
