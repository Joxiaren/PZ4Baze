#include "operacije.h"

FILE* otvoriDatoteku(char* fileName)
{
    FILE* file = fopen(fileName, "rb+");
    
    if(file == NULL) 
    {
        file = fopen(fileName, "wb+");
        if(file == NULL) 
        {
            printf("Fajl '%s' ne postoji i ne moze se kreirati\n", fileName);
            return NULL;
        }
        BLOK blok;
        blok.slogovi[0].sifraLeta = OZNAKA_KRAJA_DATOTEKE;
        fwrite(&blok, sizeof(BLOK), 1, file);

        printf("Uspesno kreiran i otvoren fajl '%s'\n", fileName);
    }
    else printf("Uspesno otvoren fajl '%s'\n", fileName);

    return file;
}
SLOG* pronadjiSlog(FILE* fajl, int sifraLeta)
{
    if(fajl == NULL) 
    {
        printf("Dati fajl nije pronadjen\n");
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
    printf("%2s %2s %8s %16s %7s %3s %4s %21s\n", "BL", "SL", "SifraL", "Datum", "TipAvio", "Trj", "Udlj", "MestoDolaska");
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

    
    printf("%08d %04d-%02d-%02d %02d:%02d %7s %03d %04d %21s",
        slog->sifraLeta,
        slog->datum.year,
        slog->datum.month,
        slog->datum.day,
        slog->datum.hour,
        slog->datum.minute,
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
                    sifraLeta = naredniBlok.slogovi[0].sifraLeta;
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
void ispisBrzAvion(FILE* fajl)
{
    char avionTip[7];
    double brzina = 0.0;

    BLOK blok;
    fseek(fajl, 0, SEEK_SET);
    while(fread(&blok, sizeof(BLOK), 1, fajl))
    {
        int j;
        for(j = 0; j < FBLOKIRANJA; j++)
        {
            if(blok.slogovi[j].sifraLeta != OZNAKA_KRAJA_DATOTEKE)
            {
                double thisBrzina = (double)blok.slogovi[j].udaljenostLeta / (double)blok.slogovi[j].trajanjeLeta;
                if(thisBrzina > brzina)
                {
                    brzina = thisBrzina;
                    strcpy(avionTip, blok.slogovi[j].tipAviona);
                }
            }
            else break;
        }
    }
    printf("Najbrzi avion je: %s sa brzinom od %lf Km/h\n", avionTip, brzina*60);
}
void dodavanjeZvezdice(FILE* fajl)
{
    int count = 0;
    BLOK blok;
    fseek(fajl, 0, SEEK_SET);
    while(fread(&blok, sizeof(BLOK), 1, fajl))
    {
        int j;
        bool updateBlock = false;
        for(j = 0; j < FBLOKIRANJA; j++)
        {
            if(blok.slogovi[j].sifraLeta == OZNAKA_KRAJA_DATOTEKE) break;
			DATETIME dt;
            memcpy(&dt, &blok.slogovi[j].datum, sizeof(DATETIME));
            //TODO Optimize
            if(blok.slogovi[j].datum.year == 2021 && nextYear(&dt, blok.slogovi[j].trajanjeLeta))
			{
				addStar(blok.slogovi[j].mestoDolaska);
                ++count;
                updateBlock = true;
			}
        }
        if(updateBlock)
        {
            fseek(fajl, -sizeof(BLOK), SEEK_CUR);
            fwrite(&blok, sizeof(BLOK), 1, fajl);
        }
    }
    printf("Pronadjeno je %d leta koji prelaze iz 2021. u 2022. godinu\n", count);
}
void ispisVrstaPoMestu(FILE* fajl)
{
    MESTA* head = NULL;
    BLOK blok;
    fseek(fajl, 0, SEEK_SET);
    while(fread(&blok, sizeof(BLOK), 1, fajl))
    {
        int j;
        for(j = 0; j < FBLOKIRANJA; j++)
        {
            if(blok.slogovi[j].sifraLeta == OZNAKA_KRAJA_DATOTEKE) break;
            MESTA* mesto = NULL;
            if((mesto = findMesto(head, blok.slogovi[j].mestoDolaska)) == NULL)
            {
                mesto = addMesto(&head, blok.slogovi[j].mestoDolaska);
            }
            if(findTip(mesto->tipovi, blok.slogovi[j].tipAviona) == NULL) 
            {
                addTip(&(mesto->tipovi), blok.slogovi[j].tipAviona);
            }
        }
    }
    while(head != NULL)
    {
        printf("\nTipovi aviona koji su sleteli u %s:\n", head->mestoDolaska);
        TIPOVI* t = head->tipovi;
        while(t != NULL)
        {
            printf("%s, ", t->tipAviona);
            t = t->nextTip;
        }
        head = head->nextMesto;
    }
    return;
}

bool leapYear(int year)
{
    if(year % 400 == 0) return true;
    if(year % 100 == 0) return false;
    if(year % 4 == 0) return true;
    return false;
}
bool nextYear(DATETIME* dateTime, int minutes)
{
    //TODO Optimize
    dateTime->minute += minutes;
    dateTime->hour += (dateTime->minute / 60);
    dateTime->day += (dateTime->hour / 24);
    int numOfDays = 28;
    int m = dateTime->month;
    if(m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) numOfDays = 31;
    else if(m == 4 || m == 6 || m == 9 || m == 11) numOfDays = 30;
    else if(leapYear(dateTime->year)) numOfDays = 29;

    if(dateTime->day > numOfDays) ++dateTime->month;

    if(dateTime->month > 12) return true;
    return false;
}
void addStar(char* s)
{
    int j = 0;
	while(s[j] != '\0') ++j;
    if(s[j-1] == '*') return; //star already exists
    s[j] = '*';
    s[j+1] = '\0';
}
MESTA* findMesto(MESTA* head, char* mesto)
{
    MESTA* locator = head;
    while(locator != NULL)
    {
        if(!strcmp(locator->mestoDolaska, mesto)) return locator;
        locator = locator->nextMesto;
    }
    return locator;
}
MESTA* addMesto(MESTA** head, char* mesto)
{
    MESTA* novoMesto = (MESTA*)malloc(sizeof(MESTA));
    novoMesto->nextMesto=NULL;
    novoMesto->tipovi=NULL;
    strcpy(novoMesto->mestoDolaska, mesto);

    MESTA* last = *head;

    if(*head == NULL)
    {
        *head = novoMesto;
        return *head;
    }
    while(last->nextMesto != NULL) last = last->nextMesto;
    last->nextMesto = novoMesto;
    return last->nextMesto;
}
TIPOVI* findTip(TIPOVI* head, char* tip)
{
    TIPOVI* locator = head;
    while(locator != NULL)
    {
        if(!strcmp(locator->tipAviona, tip)) return locator;
        locator = locator->nextTip;
    }
    return locator;
}
TIPOVI* addTip(TIPOVI** head, char* tip)
{
    TIPOVI* noviTip = (TIPOVI*) malloc(sizeof(TIPOVI));
    noviTip->nextTip = NULL;
    strcpy(noviTip->tipAviona, tip);

    TIPOVI* last = *head;

    if(*head == NULL)
    {
        *head = noviTip;
        return *head;
    }
    while(last->nextTip != NULL) last = last->nextTip;
    last->nextTip = noviTip;
    return last->nextTip;
}

