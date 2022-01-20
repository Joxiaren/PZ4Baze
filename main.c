#include "operacije.h"

void endStr(char* s)
{
    int i = 0;
    while(s[i] != '\n') ++i;
    s[i] = '\0';
}
void cleanInput()
{
    while ((getchar()) != '\n'); 
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
            printf("\n");
        }
        else if(userInput == 2)
        {
            int sifraLeta;
            printf("Unesite sifru leta za pretragu: ");
            scanf("%d", &sifraLeta);
            cleanInput();
            
            SLOG* slog = pronadjiSlog(fajl, sifraLeta);
            if(slog != NULL)
            {
                printf("ev ga slog\n");
                ispisiSlog(slog);
                printf("\n");
            }
            else printf("Dati slog nije pronadjen\n");
        }
        else if(userInput == 3)
        {
            SLOG slog;
            printf("Unesite sifru leta: ");
            scanf("%d", &slog.sifraLeta);

            printf("Unesite datum (Format: 'YYYY-MM-DD HH:mm')");
            scanf("%d-%d-%d %d:%d",
                &slog.datum.year,
                &slog.datum.month,
                &slog.datum.day,
                &slog.datum.hour,
                &slog.datum.minute);


            //TODO: Provjera datuma

            printf("Unesite tip aviona (6 char max)");
            scanf("%s", slog.tipAviona);

            printf("Unesite trajanje leta (minute)");
            scanf("%d", &slog.trajanjeLeta);
            cleanInput();

            printf("Unesite udaljenost leta (kilometri)");
            scanf("%d", &slog.udaljenostLeta);
            cleanInput();

            printf("Unesite mesto dolaska (20 char max)");
            scanf("%s", slog.mestoDolaska);

            ispisiSlog(&slog);
            printf("\n");
            dodajSlog(fajl, &slog);
            printf("\n");
        }
        else if(userInput == 4)
        {
            ispisiSveSlogove(fajl);
            printf("\n");
        }
        else if(userInput == 5)
        {
            int sifraLeta;
            printf("Unesite sifru leta za brisanje: ");
            scanf("%d", &sifraLeta);
            cleanInput();
            obrisiSlogFizicki(fajl, sifraLeta);
            printf("\n");
        }
        else if(userInput == 6) ispisBrzAvion(fajl);
        else if(userInput == 7)
        {

        }
        else if(userInput == 8)
        {

        }
        else
        {
            printf("Los input!!\n");
        }
    }
    return 0;
}