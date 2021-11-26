#ifndef STATISTICS_H
#define STATISTICS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int hp, mp, atk, def, mag, spr;
} statistics_t;

statistics_t newStatistics(int hp, int mp, int atk, int def, int mag, int spr);
void printStatistics(statistics_t *s);

#endif
