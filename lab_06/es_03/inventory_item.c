#include "inventory_item.h"

struct _inventory_item
{
    char *name, *kind;
    statistics_t statistics;
};

inventory_item_t newInventoryItem(char *name, char *kind, int hp, int mp, int atk, int def, int mag, int spr)
{
    inventory_item_t ii = malloc(sizeof(struct _inventory_item));
    ii->name = strdup(name);
    ii->kind = strdup(kind);
    ii->statistics = newStatistics(hp, mp, atk, def, mag, spr);
    return ii;
}

void freeInventoryItem(inventory_item_t ii)
{
    free(ii->name);
    free(ii->kind);
    free(ii);
}

void printInventoryItem(inventory_item_t ii)
{
    printf("nome: %s tipo: %s ", ii->name, ii->name);
    printStatistics(&ii->statistics);
}

statistics_t getInventoryItemStatistics(inventory_item_t ii)
{
    return ii->statistics;
}

char *getInventoryItemName(inventory_item_t ii)
{
    return ii->name;
}