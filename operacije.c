#include "operacije.h"

FILE* otvoriDatoteku(char* filename)
{
    FILE* file = fopen(filename, "rb+");
    
    if(file == NULL) printf("Fajl '%s' se ne moze otvoriti\n", filename);
    else printf("Uspesno otvoren fajl '%s'\n", filename);

    return file;
}

SLOG* pronadjiSlog(FILE* fajl, int sifraLeta)
{
    if(fajl == NULL) 
    {
        printf("Dati slog nije pronadjen\n");
        return NULL;
    }
    fseek(fajl, 0, SEEK_SET);
    BLOK blok;

    while(fread(&blok, sizeof(BLOK), 1, fajl))
    {
        int j;
        for(j = 0; j < FBLOKIRANJA; j++)
        {
            if(blok.slogovi[j].sifraLeta == OZNAKA_KRAJA_DATOTEKE) return NULL;
            if(blok.slogovi[j].sifraLeta == sifraLeta)
            {
                SLOG* slog = (SLOG*) malloc(sizeof(SLOG));
                memcpy(slog, &blok.slogovi[j], sizeof(SLOG));

                return slog;
            }
        }
    }
    return NULL;
}
void dodajSlog(FILE* fajl, SLOG* slog)
{
    if(fajl == NULL)
    {
        printf("Fajl nije otvoren\n");
        return;
    }

    SLOG* exists = pronadjiSlog(fajl, slog->sifraLeta);
    if(exists != NULL)
    {
        printf("Slog sa datom sifrom leta vec postoji\n");
        return;
    }

    BLOK blok;
    fseek(fajl, -sizeof(BLOK), SEEK_END);
    fread(&blok, sizeof(BLOK), 1, fajl);

    int j;
    for(j = 0; j < FBLOKIRANJA; j++)
    {
        if(blok.slogovi[j].sifraLeta == OZNAKA_KRAJA_DATOTEKE)
        {
            memcpy(&blok.slogovi[j], slog, sizeof(SLOG));
            break;
        }
    }

    ++j;
    if(j < FBLOKIRANJA)
    {
        blok.slogovi[j].sifraLeta = OZNAKA_KRAJA_DATOTEKE;
        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK), 1, fajl);
    }
    else
    {
        fseek(fajl, -sizeof(BLOK), SEEK_CUR);
        fwrite(&blok, sizeof(BLOK), 1, fajl);

        BLOK noviBlok;
        noviBlok.slogovi[0].sifraLeta = OZNAKA_KRAJA_DATOTEKE;
        fwrite(&noviBlok, sizeof(BLOK), 1, fajl);
    }
    if(ferror(fajl)) printf("Greska pirlikom upisa u fajl\n");
    else printf("Upis uspesno zavrsen\n");

    return;
}
void ispisiSveSlogove(FILE* fajl)
{
    if(fajl == NULL)
    {
        printf("Fajl nije otvoren\n");
        return;
    }

    BLOK blok;
    int rbBloka = 0;
    printf("BL SL SifraL Datum TipAviona Trajanje Udaljenost MestoDolaska\n");
    fseek(fajl, 0, SEEK_SET);
    while(fread(&blok, sizeof(BLOK), 1, fajl))
    {
        for(int j = 0; j < FBLOKIRANJA; j++)
        {
            if(blok.slogovi[j].sifraLeta == OZNAKA_KRAJA_DATOTEKE)
            {
                printf("B%d S%d *\n", rbBloka, j);
                break;
            }
            printf("B%d S%d ",rbBloka, j);
            ispisiSlog(&blok.slogovi[j]);
            printf("\n");
        }
        ++rbBloka;
    }

}
void ispisiSlog(SLOG* slog)
{
    printf("%08d %17s %7s %03d %04d %21s",
        slog->sifraLeta,
        slog->datum,
        slog->tipAviona,
        slog->trajanjeLeta,
        slog->udaljenostLeta,
        slog->mestoDolaska);
}
void obrisiSlogFizicki(FILE* fajl, int sifraLeta)
{
    SLOG* slog = pronadjiSlog(fajl, sifraLeta);
    if(slog == NULL)
    {
        printf("Slog ne postoji!\n");
        return;
    }

    BLOK blok, naredniBlok;
    int sifraLetaZaBrisanje = sifraLetaZaBrisanje;

    fseek(fajl, 0, SEEK_SET);
    while(fread(&blok, 1, sizeof(BLOK), fajl))
    {
        for(int j = 0; j < FBLOKIRANJA; j++)
        {
            if(blok.slogovi[j].sifraLeta == OZNAKA_KRAJA_DATOTEKE)
            {
                if(!j)
                {
                    printf("Skracivanje fajla...\n");
                    fseek(fajl, -sizeof(BLOK), SEEK_END);
                    long bytesToKeep = ftell(fajl);
                    ftruncate(fileno(fajl), bytesToKeep);
                    fflush(fajl);
                }
                printf("Slog je fizicki obrisan\n");
                return;
            }
            if(blok.slogovi[j].sifraLeta == sifraLeta)
            {
                for(int jj = j+1; jj < FBLOKIRANJA; jj++)
                {
                    memcpy(&(blok.slogovi[jj-1]), &(blok.slogovi[jj]), sizeof(SLOG));
                }
                int podatakaProcitano = fread(&naredniBlok, sizeof(BLOK), 1, fajl);

                if(podatakaProcitano)
                {
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    memcpy(&(blok.slogovi[FBLOKIRANJA-1]), &(naredniBlok.slogovi[0]), sizeof(SLOG));
                    naredniBlok.slogovi[0].sifraLeta = sifraLeta;
                }

                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                fseek(fajl, 0, SEEK_CUR);

                if(!podatakaProcitano)
                {
                    printf("Slog je fizicki obrisan\n");
                    return;
                }
                break;
            }
        }
        
    }

}