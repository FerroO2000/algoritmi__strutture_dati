#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define FILE_NAME "att1.txt"

typedef struct
{
    int start, end, delta;
} att_t;

void printAtt(att_t *a);
void attSel(att_t *a, int n);
int printRec(att_t *a, int *p, int idx);

int main()
{
    att_t *atts;
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL)
    {
        return 1;
    }
    int n, i;
    fscanf(fp, "%d", &n);
    atts = (att_t *)calloc(n, sizeof(att_t));
    for (i = 0; i < n; i++)
    {
        fscanf(fp, "%d %d", &atts[i].start, &atts[i].end);
        atts[i].delta = atts[i].end - atts[i].start;
    }
    fclose(fp);

    int j, swapPos;
    att_t tmpAtt;
    for (i = 0; i < (n - 1); i++)
    {
        tmpAtt = atts[i];
        swapPos = -1;
        for (j = i + 1; j < n; j++)
        {
            if ((atts[j].start < tmpAtt.start) ||
                ((atts[j].start == tmpAtt.start) && (atts[j].delta > tmpAtt.delta)))
            {
                tmpAtt = atts[j];
                swapPos = j;
            }
        }
        if (swapPos >= 0)
        {
            atts[swapPos] = atts[i];
            atts[i] = tmpAtt;
        }
    }

    printf("attivita' disponibili:\n");
    for (i = 0; i < n; i++)
    {
        printAtt(&atts[i]);
    }

    attSel(atts, n);

    free(atts);

    return 0;
}

void printAtt(att_t *a)
{
    printf(" (%d %d) ", a->start, a->end);
}

void attSel(att_t *a, int n)
{
    int d[n], p[n], i, j, last = 1, maxDelta = 0;
    d[0] = a[0].delta;
    p[0] = -1;
    for (i = 1; i < n; i++)
    {
        d[i] = a[i].delta;

        p[i] = -1;
        for (j = 0; j < i; j++)
        {
            if (!(a[j].end > a[i].start) && (d[i] < d[j] + a[i].delta))
            {
                d[i] = d[j] + a[i].delta;
                p[i] = j;
            }
        }
        if (maxDelta < d[i])
        {
            maxDelta = d[i];
            last = i;
        }
    }
    printf("\nsequenza ottimale:");
    printf("\ndurata massima: %d", printRec(a, p, last));
}

int printRec(att_t *a, int *p, int idx)
{
    if (p[idx] == -1)
    {
        printAtt(&a[idx]);
        return a[idx].delta;
    }
    int res = a[idx].delta + printRec(a, p, p[idx]);
    printAtt(&a[idx]);
    return res;
}