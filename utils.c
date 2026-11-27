#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include "utils.h"

void blad_systemowy(const char *komunikat) {
    int kod = errno;
    fprintf(stderr, "[BLAD errno=%d] ", kod);
    errno = kod;
    perror(komunikat);
    exit(EXIT_FAILURE);
}

int wczytaj_int(const char *tekst, int min, int max, const char *nazwa_parametru) {
    char *koniec = NULL;
    long wartosc;

    if (tekst == NULL || tekst[0] == '\0') {
        fprintf(stderr, "Blad walidacji: parametr '%s' jest pusty.\n", nazwa_parametru);
        exit(EXIT_FAILURE);
    }

    errno = 0;
    wartosc = strtol(tekst, &koniec, 10);

    if (errno == ERANGE || *koniec != '\0' || wartosc < min || wartosc > max) {
        fprintf(stderr, "Blad walidacji: parametr '%s' musi byc liczba calkowita z zakresu %d-%d (podano: '%s').\n",
                nazwa_parametru, min, max, tekst);
        exit(EXIT_FAILURE);
    }

    return (int)wartosc;
}

/* raczej to overengineering, bo nie ma limitu ale lepiej byc bezpiecznym */
void sprawdz_limit_procesow(int planowane_procesy) {
    struct rlimit rl;

    if (getrlimit(RLIMIT_NPROC, &rl) == -1) {
        blad_systemowy("getrlimit(RLIMIT_NPROC)");
    }

    if (rl.rlim_cur != RLIM_INFINITY && (rlim_t)planowane_procesy >= rl.rlim_cur) {
        fprintf(stderr, "Blad: planowana liczba procesow (%d) przekracza limit uzytkownika (%lu).\n",
                planowane_procesy, (unsigned long)rl.rlim_cur);
        exit(EXIT_FAILURE);
    }
}

const char *nazwa_paliwa(RodzajPaliwa p) {
    switch (p) {
        case PALIWO_BRAK:    return "BRAK";
        case PALIWO_E5:      return "E5";
        case PALIWO_E10:     return "E10";
        case PALIWO_B7:      return "B7";
        case PALIWO_B10:     return "B10";
        case PALIWO_LPG:     return "LPG";
        case PALIWO_CNG_LNG: return "CNG/LNG";
        case PALIWO_H2:      return "H2";
        case PALIWO_EV:      return "EV";
        default:             return "NIEZNANE";
    }
}

const char *nazwa_pojazdu(TypPojazdu t) {
    switch (t) {
        case POJAZD_OSOBOWY:     return "OSOBOWY";
        case POJAZD_CIEZAROWY:   return "CIEZAROWY";
        case POJAZD_AUTOBUS:     return "AUTOBUS";
        case POJAZD_ELEKTRYCZNY: return "ELEKTRYCZNY";
        default:                 return "NIEZNANY";
    }
}
