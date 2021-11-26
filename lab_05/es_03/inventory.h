#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdlib.h>
#include <string.h>
#include "statistics.h"

typedef struct
{
    char *name, *kind;
    statistics_t stats;
} object_t;

typedef struct _inventory *inventory_t;

inventory_t newInventoryFromFile(char *fileName);
void printInventory(inventory_t inv);
object_t *getInventoryObject(inventory_t inv, int objIdx);
object_t *searchInventoryObject(inventory_t inv, char *name);
void printObject(object_t *o);
void freeInventory(inventory_t inv);

#endif
