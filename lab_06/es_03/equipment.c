#include "equipment.h"

struct _equipment
{
    size_t count;
    bool itemsMask[MAX_INVENTORY_ITEMS];
    inventory_item_t items[MAX_INVENTORY_ITEMS];
};

equipment_t newEquipment()
{
    equipment_t e = malloc(sizeof(struct _equipment));
    int i;
    for (i = 0; i < MAX_INVENTORY_ITEMS; i++)
    {
        e->itemsMask[i] = false;
        e->items[i] = NULL;
    }
    e->count = 0;
    return e;
}

void freeEquipment(equipment_t e)
{
    free(e);
}

void printEquipment(equipment_t e)
{
    printf("\nequipaggiamenti assegnati:\n");
    int i;
    for (i = 0; i < MAX_INVENTORY_ITEMS; i++)
    {
        if (e->itemsMask[i])
        {
            printf("%d -> ", i);
            printInventoryItem(e->items[i]);
            printf("\n");
        }
    }
}

size_t getEquipmentCount(equipment_t e)
{
    return e->count;
}

statistics_t applyEquipment(equipment_t e, statistics_t *baseS)
{
    int i;
    statistics_t tmpStats = *baseS, modStats;
    for (i = 0; i < MAX_INVENTORY_ITEMS; i++)
    {
        if (e->itemsMask[i])
        {
            modStats = getInventoryItemStatistics(e->items[i]);
            tmpStats = calculateNewStatistics(&tmpStats, &modStats);
        }
    }
    return tmpStats;
}

void addEquipmentItem(equipment_t e, inventory_item_t ii)
{
    if (e->count >= MAX_INVENTORY_ITEMS || ii == NULL)
    {
        return;
    }
    int i, freePos;
    for (i = 0; i < MAX_INVENTORY_ITEMS; i++)
    {
        if (!e->itemsMask[i])
        {
            freePos = i;
            break;
        }
    }
    e->count++;
    e->itemsMask[freePos] = true;
    e->items[freePos] = ii;
}

void removeEquipmentItem(equipment_t e, int itemIdx)
{
    if (e->count == 0 || itemIdx < 0 || itemIdx >= MAX_INVENTORY_ITEMS)
    {
        return;
    }
    --e->count;
    e->itemsMask[itemIdx] = false;
    e->items[itemIdx] = NULL;
}