#define FBLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE -1

typedef struct {
    int sifraLeta;
    char datum[17];
    char tipAviona[7];
    int trajanjeLeta;
    int udaljenostLeta;
    char mestoDolaska[21];
} SLOG;

typedef struct Blok {
    SLOG slogovi[FBLOKIRANJA];
} BLOK;

typedef struct DateTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
} DATETIME;