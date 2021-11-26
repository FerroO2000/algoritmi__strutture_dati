#include "statistics.h"

statistics_t newStatistics(int hp, int mp, int atk, int def, int mag, int spr)
{
    statistics_t s;
    s.hp = hp;
    s.mp = mp;
    s.atk = atk;
    s.def = def;
    s.mag = mag;
    s.spr = spr;
    return s;
}

void printStatistics(statistics_t *s)
{
    printf("hp: %d mp: %d atk: %d def: %d mag: %d spr: %d", s->hp, s->mp, s->atk, s->def, s->mag, s->spr);
}