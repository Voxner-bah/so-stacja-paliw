#ifndef UTILS_H
#define UTILS_H

#include "common.h"

/* obsluga bledu funkcji systemowej z uzyciem errno i perror */
void blad_systemowy(const char *komunikat);

/* gupia walidacja i konwersja liczby calkowitej z zakresu [min, max] */
int wczytaj_int(const char *tekst, int min, int max, const char *nazwa_parametru);

/* sprawdzenie limitu procesow uzytkownika (RLIMIT_NPROC) */
void sprawdz_limit_procesow(int planowane_procesy);

/* Nazwy tekstowe do logow i diagnostyki */
const char *nazwa_paliwa(RodzajPaliwa p);
const char *nazwa_pojazdu(TypPojazdu t);

#endif
