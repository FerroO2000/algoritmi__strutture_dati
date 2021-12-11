#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "inventory_item.h"

#define MAX_INVENTORY_ITEMS 8

typedef struct _equipment *equipment_t;

equipment_t newEquipment();
void freeEquipment(equipment_t e);
void printEquipment(equipment_t e);
size_t getEquipmentCount(equipment_t e);
statistics_t applyEquipment(equipment_t e, statistics_t *baseS);
void addEquipmentItem(equipment_t e, inventory_item_t ii);
void removeEquipmentItem(equipment_t e, int itemIdx);

#endif