#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdlib.h>
#include <string.h>
#include "inventory_item.h"

#define STR_BUFF_SIZE 51

typedef struct _inventory *inventory_t;

inventory_t newInventoryFromFile(char *fileName);
void freeInventory(inventory_t inv);
void printInventory(inventory_t inv);
inventory_item_t getInventoryItemAt(inventory_t inv, int itemIdx);

#endif
