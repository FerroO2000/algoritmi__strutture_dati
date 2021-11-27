#include "inventory.h"

#define STR_BUFF_SIZE 51

struct _inventory
{
    object_t *objects;
    size_t objectCount;
};

object_t _newObject(char *tmpName, char *tmpKind, int hp, int mp, int atk, int def, int mag, int spr);
void _freeObject(object_t *o);

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
    inv->objectCount = nInv;
    inv->objects = (object_t *)calloc(nInv, sizeof(object_t));

    for (i = 0; i < nInv; i++)
    {
        fscanf(fp, "%s %s %d %d %d %d %d %d\n", tmpName, tmpKind, &hp, &mp, &atk, &def, &mag, &spr);
        inv->objects[i] = _newObject(tmpName, tmpKind, hp, mp, atk, def, mag, spr);
    }

    fclose(fp);

    return inv;
}

void freeInventory(inventory_t inv)
{
    int i;
    for (i = 0; i < inv->objectCount; i++)
    {
        _freeObject(&inv->objects[i]);
    }
    free(inv->objects);
    free(inv);
}

void printInventory(inventory_t inv)
{
    int i;
    for (i = 0; i < inv->objectCount; i++)
    {
        printf("%d -> ", i);
        printObject(&inv->objects[i]);
        printf("\n");
    }
}

void printObject(object_t *o)
{
    printf("nome: %s tipo: %s ", o->name, o->name);
    printStatistics(&o->stats);
}

object_t *getInventoryObject(inventory_t inv, int objIdx)
{
    return &inv->objects[objIdx];
}

object_t *searchInventoryObject(inventory_t inv, char *name)
{
    int i;
    for (i = 0; i < inv->objectCount; i++)
    {
        if (strcmp(name, inv->objects[i].name) == 0)
        {
            return &inv->objects[i];
        }
    }
    return NULL;
}

object_t _newObject(char *tmpName, char *tmpKind, int hp, int mp, int atk, int def, int mag, int spr)
{
    object_t o;
    o.name = strdup(tmpName);
    o.kind = strdup(tmpKind);
    o.stats = newStatistics(hp, mp, atk, def, mag, spr);
    return o;
}

void _freeObject(object_t *o)
{
    free(o->name);
    free(o->kind);
}