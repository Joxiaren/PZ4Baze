#define FBLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE -1
typedef struct DateTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
} DATETIME;

typedef struct {
    int sifraLeta;
    DATETIME datum;
    char tipAviona[7];
    int trajanjeLeta;
    int udaljenostLeta;
    char mestoDolaska[21];
} SLOG;

typedef struct Blok {
    SLOG slogovi[FBLOKIRANJA];
} BLOK;

typedef struct tipovi{
    char tipAviona[7];
    struct tipovi* nextTip;
} TIPOVI;

typedef struct mesta {
    char mestoDolaska[21];
    TIPOVI* tipovi;
    struct mesta* nextMesto;
} MESTA;



