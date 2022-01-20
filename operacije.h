#ifndef OPERACIJE_H
#define OPERACIJE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include "slog.h"

FILE* otvoriDatoteku(char *filename);
SLOG* pronadjiSlog(FILE *fajl, int sifraLeta);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void ispisiSlog(SLOG* slog);
void obrisiSlogFizicki(FILE *fajl, int sifraLeta);

#endif