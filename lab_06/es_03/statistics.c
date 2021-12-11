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

statistics_t calculateNewStatistics(statistics_t *base, statistics_t *mod)
{
    int hp = base->hp + mod->hp,
        mp = base->mp + mod->mp,
        atk = base->atk + mod->atk,
        def = base->def + mod->def,
        mag = base->mag + mod->mag,
        spr = base->spr + mod->spr;
    if (hp <= 0)
        hp = 1;
    if (mp <= 0)
        mp = 1;
    if (atk <= 0)
        atk = 1;
    if (def <= 0)
        def = 1;
    if (mag <= 0)
        mag = 1;
    if (spr <= 0)
        spr = 1;
    return newStatistics(hp, mp, atk, def, mag, spr);
}