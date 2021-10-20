#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STR 30
#define NOME_FILE "corse.txt"
#define MAX_TRATTE 1000
#define CMD_NUM 6

typedef enum
{
    r_date,
    r_partenza,
    r_capolinea,
    r_ritardo,
    r_ritardo_tot,
    r_fine
} comando_e;

typedef struct
{
    int ora, minuto, secondo;
} Tempo;

typedef struct
{
    int anno, mese, giorno;
} Data;

typedef struct
{
    char codice[MAX_STR + 1], partenza[MAX_STR + 1], destinazione[MAX_STR + 1];
    Data data;
    Tempo oraPartenza, oraArrivo;
    size_t ritardo;
} Tratta;

Tratta *leggiTratte(size_t *numTratte);
Tempo leggiTempo(char *orario);
Data leggiData(char *data);
Data leggiDataUtente();
void stampaTratta(Tratta *t);
int comparaData(Data in, Data compara);
void elencaIntervalloDate(Tratta *t, size_t nTratte, Data iData, Data fDate, bool ritardo);
void elencaPartenze(Tratta *t, size_t nTratte, char *partenza);
void elencaDestinazioni(Tratta *t, size_t nTratte, char *destinazione);
void ritardoTratta(Tratta *t, size_t nTratte, char *codice);
comando_e leggiComando(char (*comandi)[MAX_STR + 1]);
void stampaComandi(char (*comandi)[MAX_STR + 1]);

int main()
{
    char comandi[][MAX_STR + 1] = {"date", "partenza", "capolinea", "ritardo", "ritardo_tot", "fine"};

    size_t nTratte;
    Tratta *tratte = leggiTratte(&nTratte);

    comando_e cmd;
    bool stop = false;
    char cod[MAX_STR + 1], part[MAX_STR + 1], cap[MAX_STR + 1];
    Data iData, fData, iDataR, fDataR;
    do
    {
        stampaComandi(comandi);
        cmd = leggiComando(comandi);
        switch (cmd)
        {
        case r_date:
            printf("inserire prima data [aaaa/mm/gg]: ");
            iData = leggiDataUtente();
            printf("inserire seconda data [aaaa/mm/gg]: ");
            fData = leggiDataUtente();
            elencaIntervalloDate(tratte, nTratte, iData, fData, false);
            break;

        case r_partenza:
            printf("inserire luogo di partenza: ");
            scanf("%s", part);
            elencaPartenze(tratte, nTratte, part);
            break;

        case r_capolinea:
            printf("inserire luogo di destinazione: ");
            scanf("%s", cap);
            elencaDestinazioni(tratte, nTratte, cap);
            break;

        case r_ritardo:
            printf("inserire prima data [aaaa/mm/gg]: ");
            iDataR = leggiDataUtente();
            printf("inserire seconda data [aaaa/mm/gg]: ");
            fDataR = leggiDataUtente();
            elencaIntervalloDate(tratte, nTratte, iDataR, fDataR, true);
            break;

        case r_ritardo_tot:
            printf("inserire codice della tratta: ");
            scanf("%s", cod);
            ritardoTratta(tratte, nTratte, cod);
            break;

        case r_fine:
            stop = true;
            break;

        default:
            break;
        }
    } while (!stop);

    return 0;
}

Tratta *leggiTratte(size_t *numTratte)
{
    FILE *fIn = fopen(NOME_FILE, "r");
    if (fIn == NULL)
    {
        exit(1);
    }

    int tmpNum;
    fscanf(fIn, "%d", &tmpNum);
    Tratta *t = calloc(tmpNum, sizeof(Tratta));

    if (tmpNum > MAX_TRATTE)
    {
        tmpNum = MAX_TRATTE;
    }

    size_t i;
    char tmpOraPartenza[MAX_STR + 1], tmpOraArrivo[MAX_STR + 1], data[MAX_STR + 1];
    for (i = 0; i < tmpNum; i++)
    {
        fscanf(fIn, "%s %s %s %s %s %s %d\n", t[i].codice, t[i].partenza, t[i].destinazione, data, tmpOraPartenza, tmpOraArrivo, &t[i].ritardo);
        t[i].oraPartenza = leggiTempo(tmpOraPartenza);
        t[i].oraArrivo = leggiTempo(tmpOraArrivo);
        t[i].data = leggiData(data);
    }

    fclose(fIn);

    *numTratte = tmpNum;
    return t;
}

Tempo leggiTempo(char *orario)
{
    Tempo t;
    sscanf(orario, "%d:%d:%d", &t.ora, &t.minuto, &t.secondo);

    return t;
}

Data leggiData(char *data)
{
    Data d;
    sscanf(data, "%d/%d/%d", &d.anno, &d.mese, &d.giorno);
    return d;
}

Data leggiDataUtente()
{
    Data d;
    scanf("%d/%d/%d", &d.anno, &d.mese, &d.giorno);
    return d;
}

comando_e leggiComando(char (*comandi)[MAX_STR + 1])
{
    char cmd[MAX_STR + 1];
    printf("comando: ");
    scanf("%s", cmd);
    size_t i;
    for (i = 0; i < CMD_NUM; i++)
    {
        if (strcmp(cmd, comandi[i]) == 0)
        {
            return (comando_e)i;
        }
    }
    return -1;
}

void stampaComandi(char (*comandi)[MAX_STR + 1])
{
    int i;
    printf("comandi disponibili:");
    for (i = 0; i < CMD_NUM; i++)
    {
        printf(" %s", comandi[i]);
    }
    printf("\n");
}

int comparaData(Data in, Data compara)
{
    if (in.anno == compara.anno && in.mese == compara.mese && in.giorno == compara.giorno)
    {
        return 0;
    }
    if (in.anno > compara.anno)
    {
        return 1;
    }
    if (in.anno == compara.anno)
    {
        if (in.mese > compara.mese)
        {
            return 1;
        }
        if (in.mese == compara.mese)
        {
            if (in.giorno >= compara.giorno)
            {
                return 1;
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

void stampaTratta(Tratta *t)
{
    printf("codice -> %s\npartenza -> %s\ndestinazione -> %s\ndata -> %d/%02d/%02d\nora partenza -> %02d:%02d:%02d\nora arrivo -> %02d:%02d:%02d\nritardo -> %d\n\n",
           t->codice, t->partenza, t->destinazione, t->data.anno, t->data.mese, t->data.giorno, t->oraPartenza.ora, t->oraPartenza.minuto, t->oraPartenza.secondo,
           t->oraArrivo.ora, t->oraArrivo.minuto, t->oraArrivo.secondo, t->ritardo);
}

void elencaPartenze(Tratta *t, size_t nTratte, char *partenza)
{
    size_t i;
    for (i = 0; i < nTratte; i++)
    {
        if (strcmp(t[i].partenza, partenza) == 0)
        {
            stampaTratta(&t[i]);
        }
    }
}

void elencaDestinazioni(Tratta *t, size_t nTratte, char *destinazione)
{
    size_t i;
    for (i = 0; i < nTratte; i++)
    {
        if (strcmp(t[i].destinazione, destinazione) == 0)
        {
            stampaTratta(&t[i]);
        }
    }
}

void elencaIntervalloDate(Tratta *t, size_t nTratte, Data iData, Data fDate, bool ritardo)
{
    size_t i;
    for (i = 0; i < nTratte; i++)
    {
        if (comparaData(t[i].data, iData) >= 0 && comparaData(t[i].data, fDate) <= 0)
        {
            if (ritardo && t[i].ritardo <= 0)
            {
                continue;
            }
            stampaTratta(&t[i]);
        }
    }
}

void ritardoTratta(Tratta *t, size_t nTratte, char *codice)
{
    size_t i, rit = 0;
    for (i = 0; i < nTratte; i++)
    {
        if (strcmp(codice, t[i].codice) == 0)
        {
            rit += t[i].ritardo;
        }
    }
    printf("ritardo %s -> %d\n", codice, rit);
}