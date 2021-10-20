#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_STR 30
#define NOME_FILE "corse.txt"
#define CMD_NUM 8
#define MOD_NUM 2
#define MIN_MATCH 1

typedef enum
{
    e_Stampa,
    e_OrdinaDate,
    e_ordinaCodice,
    e_OrdinaPartenza,
    e_OrdinaArrivo,
    e_CercaPartenza,
    e_RicaricaFile,
    e_Fine,
} EnumComandi;

typedef enum
{
    e_Video,
    e_File,
} EnumMod;

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

Tratta *leggiTratte(size_t *numTratte, char *file);
Tempo leggiTempo(char *orario);
Data leggiData(char *data);
void stampaTratta(Tratta *t);
void stampaListaTratte(Tratta **tt, size_t nT);
int comparaData(Data in, Data compara);
int comparaTempo(Tempo in, Tempo compara);
EnumComandi leggiComando(char (*comandi)[MAX_STR + 1]);
EnumMod leggiMod(char (*mods)[MAX_STR + 1]);
void stampaComandi(char (*comandi)[MAX_STR + 1]);
void cercaPartenze(Tratta *t, size_t nT, char *query);
void stampa(Tratta *t, size_t nT, EnumMod mod);
void normalizzaStringa(char *in, char *ris);

Tratta **ordinaDate(Tratta *t, size_t nT);
Tratta **ordinaCodici(Tratta *t, size_t nT);
Tratta **ordinaArrivi(Tratta *t, size_t nT);
Tratta **ordinaPartenze(Tratta *t, size_t nT);

int main()
{
    char comandi[][MAX_STR + 1] = {"stampa", "data", "codice", "partenza", "capolinea", "cerca", "ricarica-file", "fine"};
    char mods[][MAX_STR + 1] = {"video", "file"};

    size_t nTratte;
    Tratta *tratte = leggiTratte(&nTratte, NOME_FILE);

    Tratta **tDate = ordinaDate(tratte, nTratte);
    Tratta **tCodici = ordinaCodici(tratte, nTratte);
    Tratta **tArrivi = ordinaArrivi(tratte, nTratte);
    Tratta **tPartenze = ordinaPartenze(tratte, nTratte);

    EnumMod mod;
    EnumComandi cmd;
    bool stop = false;
    char query[MAX_STR + 1], nFile[MAX_STR + 1];
    do
    {
        stampaComandi(comandi);
        cmd = leggiComando(comandi);
        switch (cmd)
        {

        case e_OrdinaDate:
            stampaListaTratte(tDate, nTratte);
            break;

        case e_OrdinaPartenza:
            stampaListaTratte(tPartenze, nTratte);
            break;

        case e_ordinaCodice:
            stampaListaTratte(tCodici, nTratte);
            break;

        case e_OrdinaArrivo:
            stampaListaTratte(tArrivi, nTratte);
            break;

        case e_Stampa:
            mod = leggiMod(mods);
            stampa(tratte, nTratte, mod);
            break;

        case e_CercaPartenza:
            printf("chiave: ");
            scanf("%s", query);
            cercaPartenze(tratte, nTratte, query);
            break;

        case e_RicaricaFile:
            printf("nuovo file: ");
            scanf("%s", nFile);
            free(tratte);
            tratte = leggiTratte(&nTratte, nFile);
            break;

        case e_Fine:
            stop = true;
            break;

        default:
            break;
        }
    } while (!stop);

    free(tratte);

    return 0;
}

Tratta *leggiTratte(size_t *numTratte, char *file)
{
    FILE *fIn = fopen(file, "r");
    if (fIn == NULL)
    {
        exit(1);
    }

    int tmpNum;
    fscanf(fIn, "%d", &tmpNum);
    Tratta *t = calloc(tmpNum, sizeof(Tratta));

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

EnumComandi leggiComando(char (*comandi)[MAX_STR + 1])
{
    char cmd[MAX_STR + 1];
    printf("comando: ");
    scanf("%s", cmd);
    size_t i;
    for (i = 0; i < CMD_NUM; i++)
    {
        if (strcmp(cmd, comandi[i]) == 0)
        {
            return (EnumComandi)i;
        }
    }
    return -1;
}

EnumMod leggiMod(char (*mods)[MAX_STR + 1])
{
    char mod[MAX_STR + 1];
    printf("scegliere modalita' [");
    size_t i;
    for (i = 0; i < MOD_NUM; i++)
    {
        if (i == 0)
        {
            printf("%s", mods[i]);
        }
        else
        {
            printf(", %s", mods[i]);
        }
    }
    printf("]: ");
    scanf("%s", mod);
    for (i = 0; i < MOD_NUM; i++)
    {
        if (strcmp(mod, mods[i]) == 0)
        {
            return (EnumMod)i;
        }
    }
    return -1;
}

void stampaComandi(char (*comandi)[MAX_STR + 1])
{
    int i;
    printf("comandi disponibili: [");
    for (i = 0; i < CMD_NUM; i++)
    {
        if (i == 0)
        {
            printf("%s", comandi[i]);
        }
        else
        {
            printf(", %s", comandi[i]);
        }
    }
    printf("]\n");
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
            if (in.giorno > compara.giorno)
            {
                return 1;
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

int comparaTempo(Tempo in, Tempo compara)
{
    if (in.ora == compara.ora && in.minuto == compara.minuto && in.secondo == compara.secondo)
    {
        return 0;
    }
    if (in.ora > compara.ora)
    {
        return 1;
    }
    if (in.ora == compara.ora)
    {
        if (in.minuto > compara.minuto)
        {
            return 1;
        }
        if (in.minuto == compara.minuto)
        {
            if (in.secondo > compara.secondo)
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

void normalizzaStringa(char *in, char *ris)
{
    size_t i, lIn = strlen(in);
    for (i = 0; i < lIn; i++)
    {
        ris[i] = tolower(in[i]);
    }
    ris[lIn] = '\n';
}

void cercaPartenze(Tratta *t, size_t nT, char *query)
{
    char lQuery[MAX_STR + 1], tmp[MAX_STR + 1];
    normalizzaStringa(query, lQuery);
    size_t i, j, nMatch;
    for (i = 0; i < nT; i++)
    {
        normalizzaStringa(t[i].partenza, tmp);
        if (strcmp(tmp, lQuery) == 0)
        {
            stampaTratta(&t[i]);
            continue;
        }
        nMatch = 0;
        for (j = 0; j < strlen(tmp); j++)
        {
            if (tmp[j] != lQuery[j])
            {
                break;
            }
            nMatch++;
        }
        if (nMatch >= MIN_MATCH)
        {
            stampaTratta(&t[i]);
        }
    }
}

void stampa(Tratta *t, size_t nT, EnumMod mod)
{
    size_t i;
    FILE *fp;
    switch (mod)
    {
    case e_Video:
        for (i = 0; i < nT; i++)
        {
            stampaTratta(&t[i]);
        }
        break;

    case e_File:;
        fp = fopen("risultati.txt", "w");
        if (fp == NULL)
        {
            exit(1);
        }

        for (i = 0; i < nT; i++)
        {
            fprintf(fp, "%s %s %s %d/%02d/%02d %02d/%02d/%02d %02d/%02d/%02d %d\n",
                    t[i].codice, t[i].partenza, t[i].destinazione, t[i].data.anno, t[i].data.mese, t[i].data.giorno, t[i].oraPartenza.ora, t[i].oraPartenza.minuto, t[i].oraPartenza.secondo,
                    t[i].oraArrivo.ora, t[i].oraArrivo.minuto, t[i].oraArrivo.secondo, t[i].ritardo);
        }

        fclose(fp);

        break;

    default:
        break;
    }
}

void stampaListaTratte(Tratta **tt, size_t nT)
{
    size_t i;
    for (i = 0; i < nT; i++)
    {
        stampaTratta(tt[i]);
    }
}

Tratta **ordinaDate(Tratta *t, size_t nT)
{
    Tratta **ordT;
    ordT = (Tratta **)calloc(nT, sizeof(Tratta *));
    size_t i, j, offset;
    for (i = 0; i < nT; i++)
    {
        offset = 0;
        for (j = 0; j < nT; j++)
        {
            if (i == j)
            {
                continue;
            }
            if ((comparaData(t[i].data, t[j].data) > 0) || (comparaData(t[i].data, t[j].data) == 0 && comparaTempo(t[i].oraPartenza, t[j].oraPartenza) > 0))
            {
                offset++;
            }
        }
        while (ordT[offset] != NULL)
        {
            offset++;
        }
        ordT[offset] = &t[i];
    }
    return ordT;
}

Tratta **ordinaCodici(Tratta *t, size_t nT)
{
    Tratta **ordT;
    ordT = (Tratta **)calloc(nT, sizeof(Tratta *));
    size_t i, j, offset;
    for (i = 0; i < nT; i++)
    {
        offset = 0;
        for (j = 0; j < nT; j++)
        {
            if (i == j)
            {
                continue;
            }
            if (strcmp(t[i].codice, t[j].codice) > 0)
            {
                offset++;
            }
        }
        while (ordT[offset] != NULL)
        {
            offset++;
        }
        ordT[offset] = &t[i];
    }
    return ordT;
}

Tratta **ordinaArrivi(Tratta *t, size_t nT)
{
    Tratta **ordT;
    ordT = (Tratta **)calloc(nT, sizeof(Tratta *));
    size_t i, j, offset;
    for (i = 0; i < nT; i++)
    {
        offset = 0;
        for (j = 0; j < nT; j++)
        {
            if (i == j)
            {
                continue;
            }
            if (strcmp(t[i].destinazione, t[j].destinazione) > 0)
            {
                offset++;
            }
        }
        while (ordT[offset] != NULL)
        {
            offset++;
        }
        ordT[offset] = &t[i];
    }
    return ordT;
}

Tratta **ordinaPartenze(Tratta *t, size_t nT)
{
    Tratta **ordT;
    ordT = (Tratta **)calloc(nT, sizeof(Tratta *));
    size_t i, j, offset;
    for (i = 0; i < nT; i++)
    {
        offset = 0;
        for (j = 0; j < nT; j++)
        {
            if (i == j)
            {
                continue;
            }
            if (strcmp(t[i].partenza, t[j].partenza) > 0)
            {
                offset++;
            }
        }
        while (ordT[offset] != NULL)
        {
            offset++;
        }
        ordT[offset] = &t[i];
    }
    return ordT;
}
