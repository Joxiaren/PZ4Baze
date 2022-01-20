#include "operacije.h"



void strToDT(char* date, DATETIME* dateTime)
{
    
}

int main()
{
    bool running = true;
    int userInput;

    FILE* fajl = NULL;

    while(running)
    {
        printf("Odaberite opciju:\n");
        printf("1 - Otvori datoteku\n");
        printf("2 - Pretrazi datoteku\n");
        printf("3 - Dodaj slog\n");
        printf("4 - Ispisi sve slogove\n");
        printf("5 - (fizicki) Izbrisi slog\n");
        printf("6 - Ispis aviona sa najvecom brzinom\n");
        printf("7 - Dodavanje '*' na letove koji prelaze godinu\n");
        printf("8 - Prikaz razlicitih aviona za svako mesto dolaska\n");
        printf("0 - Izlaz\n");
        if(fajl == NULL) printf("!!!! Fajl nije otvoren !!!!\n");

        scanf("%d", &userInput);
        getc(stdin);

        if(userInput == 0)
        {
            running = false;
            if(fajl != NULL) fclose(fajl);
        }
        else if(userInput == 1)
        {
            char fileName[256];
            printf("Unesite ime datoteke za otvaranje: ");
            scanf("%s", fileName);
            
            fajl = otvoriDatoteku(fileName);
        }
        else if(userInput == 2)
        {
            int sifraLeta;
            printf("Unesite sifru leta za pretragu: ");
            scanf("%d", &sifraLeta);
            
            SLOG* slog = pronadjiSlog(fajl, sifraLeta);
            if(slog != NULL)
            {
                printf("ev ga slog\n");
                ispisiSlog(slog);
                printf("\n");
            }
        }
    }
    return 0;
}