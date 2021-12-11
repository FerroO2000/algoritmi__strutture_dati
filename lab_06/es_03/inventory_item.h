#ifndef INVENTORY_ITEM_H
#define INVENTORY_ITEM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "statistics.h"

typedef struct _inventory_item *inventory_item_t;

inventory_item_t newInventoryItem(char *name, char *kind, int hp, int mp, int atk, int def, int mag, int spr);
void freeInventoryItem(inventory_item_t ii);
void printInventoryItem(inventory_item_t ii);
statistics_t getInventoryItemStatistics(inventory_item_t ii);
char *getInventoryItemName(inventory_item_t ii);

#endif