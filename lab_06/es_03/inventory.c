#include "inventory.h"

struct _inventory
{
    inventory_item_t *inventoryItems;
    size_t inventoryItemCount;
};

inventory_t newInventoryFromFile(char *fileName)
{
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        return NULL;
    }

    inventory_t inv = malloc(sizeof(struct _inventory));
    char tmpKind[STR_BUFF_SIZE], tmpName[STR_BUFF_SIZE];
    int nInv, i, hp, mp, atk, def, mag, spr;
    fscanf(fp, "%d", &nInv);
    inv->inventoryItemCount = nInv;
    inv->inventoryItems = (inventory_item_t *)calloc(nInv, sizeof(inventory_item_t));

    for (i = 0; i < nInv; i++)
    {
        fscanf(fp, "%s %s %d %d %d %d %d %d\n", tmpName, tmpKind, &hp, &mp, &atk, &def, &mag, &spr);
        inv->inventoryItems[i] = newInventoryItem(tmpName, tmpKind, hp, mp, atk, def, mag, spr);
    }

    fclose(fp);

    return inv;
}

void freeInventory(inventory_t inv)
{
    int i;
    for (i = 0; i < inv->inventoryItemCount; i++)
    {
        freeInventoryItem(inv->inventoryItems[i]);
    }
    free(inv->inventoryItems);
    free(inv);
}

void printInventory(inventory_t inv)
{
    int i;
    for (i = 0; i < inv->inventoryItemCount; i++)
    {
        printf("%d -> ", i);
        printInventoryItem(inv->inventoryItems[i]);
        printf("\n");
    }
}

inventory_item_t getInventoryItemAt(inventory_t inv, int itemIdx)
{
    if (itemIdx < 0 || itemIdx >= inv->inventoryItemCount)
    {
        return NULL;
    }
    return inv->inventoryItems[itemIdx];
}
