#include "character.h"

struct _character
{
    char *cod, *name, *class;
    statistics_t statistics;
    equipment_t equipment;
};

character_t newCharacter(char *tmpCod, char *tmpName, char *tmpClass, int hp, int mp, int atk, int def, int mag, int spr)
{
    character_t c = malloc(sizeof(struct _character));
    c->cod = strdup(tmpCod);
    c->name = strdup(tmpName);
    c->class = strdup(tmpClass);
    c->statistics = newStatistics(hp, mp, atk, def, mag, spr);
    c->equipment = newEquipment();
    return c;
}

void freeCharacter(character_t c)
{
    free(c->cod);
    free(c->name);
    free(c->class);
    freeEquipment(c->equipment);
    free(c);
}

void printCharacter(character_t c)
{
    printf("codice: %s nome: %s classe: %s \n", c->cod, c->name, c->class);
    if (getEquipmentCount(c->equipment) == 0)
    {
        printStatistics(&c->statistics);
        printf("\n");
        return;
    }
    printf("statistiche di base:\n");
    printStatistics(&c->statistics);
    printEquipment(c->equipment);
    printf("statistiche con equipaggiamento:\n");
    statistics_t newStats = applyEquipment(c->equipment, &c->statistics);
    printStatistics(&newStats);
    printf("\n");
}

void addCharacterEquipment(character_t c, inventory_t inv, int itemIdx)
{
    addEquipmentItem(c->equipment, getInventoryItemAt(inv, itemIdx));
}

void removeCharacterEquipment(character_t c, int equipItemIdx)
{
    removeEquipmentItem(c->equipment, equipItemIdx);
}

bool matchCharacter(character_t c, char *cod)
{
    return strcmp(c->cod, cod) == 0;
}

void printCharacterEquipment(character_t c)
{
    printEquipment(c->equipment);
}
