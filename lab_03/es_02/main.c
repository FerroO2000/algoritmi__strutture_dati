#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STR 256
#define FILE_NAME "brani.txt"

typedef struct
{
    int nTrack;
    char **tracks;
} person_t;

person_t *readFile(int *nPerson);
void freePerson(person_t *p, int nP);
int combineRec(person_t *p, int nP, int currPos, char **sol, int count);
int combine(person_t *p, int nP);

int main()
{
    int nPerson;
    person_t *p = readFile(&nPerson);

    printf("combinazioni: %d", combine(p, nPerson));

    freePerson(p, nPerson);

    return 0;
}

person_t *readFile(int *nPerson)
{
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL)
    {
        exit(1);
    }

    person_t *p;
    int nP, nT, i, j;
    char tmpStr[MAX_STR];
    fscanf(fp, "%d", &nP);
    p = (person_t *)calloc(nP, sizeof(person_t));

    for (i = 0; i < nP; i++)
    {
        fscanf(fp, "%d", &nT);
        p[i].nTrack = nT;
        p[i].tracks = (char **)calloc(nT, sizeof(char *));

        for (j = 0; j < nT; j++)
        {
            fscanf(fp, "%s", tmpStr);
            p[i].tracks[j] = strdup(tmpStr);
        }
    }

    fclose(fp);

    *nPerson = nP;
    return p;
}

int combine(person_t *p, int nP)
{
    char **sol;
    sol = (char **)calloc(nP, sizeof(char *));
    int count = combineRec(p, nP, 0, sol, 0), i;
    for (i = 0; i < nP; i++)
    {
        free(sol[i]);
    }
    free(sol);
    return count;
}

int combineRec(person_t *p, int nP, int currPos, char **sol, int count)
{
    int i, j;
    if (currPos == (nP - 1))
    {
        for (i = 0; i < p[currPos].nTrack; i++)
        {
            for (j = 0; j < (nP - 1); j++)
            {
                printf("%s ", sol[j]);
            }
            printf("%s\n\n", p[currPos].tracks[i]);
            count++;
        }
        return count;
    }

    for (i = 0; i < p[currPos].nTrack; i++)
    {
        sol[currPos] = strdup(p[currPos].tracks[i]);
        count = combineRec(p, nP, currPos + 1, sol, count);
    }
    return count;
}

void freePerson(person_t *p, int nP)
{
    int i, j;
    for (i = 0; i < nP; i++)
    {
        for (j = 0; j < p[i].nTrack; j++)
        {
            free(p[i].tracks[j]);
        }
    }
    free(p);
}
