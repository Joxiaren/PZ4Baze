#ifndef OPERACIJE_H
#define OPERACIJE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include "slog.h"

bool leapYear(int year);
bool nextYear(DATETIME* dateTime, int minutes);
void addStar(char* s);
MESTA* findMesto(MESTA* head, char* s);
MESTA* addMesto(MESTA** head, char* s);
TIPOVI* findTip(TIPOVI* head, char* s);
TIPOVI* addTip(TIPOVI** head, char* s);

FILE* otvoriDatoteku(char *filename);
SLOG* pronadjiSlog(FILE *fajl, int sifraLeta);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void ispisiSlog(SLOG* slog);
void ispisiHeader();
void obrisiSlogFizicki(FILE *fajl, int sifraLeta);
void ispisBrzAvion(FILE* fajl);
void dodavanjeZvezdice(FILE* fajl);
void ispisVrstaPoMestu(FILE* fajl);

#endif