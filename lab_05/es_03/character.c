#include "character.h"

#define FILE_LINE_BUFF_SIZE 1024

#define MAX_EQUIP 8

struct _equip
{
    size_t count;
    bool slots[MAX_EQUIP];
    object_t *objects[MAX_EQUIP];
};

struct _character
{
    char *cod, *name, *class;
    statistics_t stats;
    struct _equip equip;
};

struct _characterList
{
    linked_list_t list;
    size_t count;
};

character_t _newCharacter(char *tmpCod, char *tmpName, char *tmpClass, int hp, int mp, int atk, int def, int mag, int spr);
void _printCharacterFunction(ll_item_key_t inC);
bool _matchCharacterFunction(ll_item_key_t cod, ll_item_t c);
struct _equip _newEquip();

character_list_t newCharacterListFromFile(char *fileName)
{
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        return NULL;
    }

    int nC = 0, hp, mp, atk, def, mag, spr;
    char tmpCod[STR_BUFF_SIZE], tmpName[STR_BUFF_SIZE], tmpClass[STR_BUFF_SIZE], lineBuff[FILE_LINE_BUFF_SIZE];
    character_list_t cl = malloc(sizeof(struct _characterList));
    cl->list = newLinkedList();

    while (fgets(lineBuff, FILE_LINE_BUFF_SIZE, fp))
    {
        sscanf(lineBuff, "%s %s %s %d %d %d %d %d %d\n", tmpCod, tmpName, tmpClass, &hp, &mp, &atk, &def, &mag, &spr);
        pushItem(cl->list, _newCharacter(tmpCod, tmpName, tmpClass, hp, mp, atk, def, mag, spr));
        nC++;
    }
    cl->count = nC;

    fclose(fp);

    return cl;
}

void freeCharacterList(character_list_t cl)
{
    freeLinkedList(cl->list);
    free(cl);
}

void printCharacterList(character_list_t cl)
{
    printLinkedList(cl->list, _printCharacterFunction);
}

void addCharacter(character_list_t cl, char *cod, char *name, char *class, int hp, int mp, int atk, int def, int mag, int spr)
{
    pushItem(cl->list, _newCharacter(cod, name, class, hp, mp, atk, def, mag, spr));
    cl->count++;
}

void removeCharacter(character_list_t cl, char *cod)
{
    removeItem(cl->list, cod, _matchCharacterFunction);
    --cl->count;
}

character_t getCharacter(character_list_t cl, char *cod)
{
    character_t c = (character_t)getItem(cl->list, cod, _matchCharacterFunction);
    return c;
}

void printCharacter(character_t c)
{
    printf("codice: %s nome: %s classe: %s ", c->cod, c->name, c->class);
    if (c->equip.count == 0)
    {
        printStatistics(&c->stats);
        printf("\n");
        return;
    }
    printf("statistiche di base:\n");
    printStatistics(&c->stats);
    printEquip(c);
    int i, hp = c->stats.hp, mp = c->stats.mp, atk = c->stats.atk,
           def = c->stats.def, mag = c->stats.mag, spr = c->stats.spr;
    for (i = 0; i < MAX_EQUIP; i++)
    {
        if (c->equip.slots[i])
        {
            hp += c->equip.objects[i]->stats.hp;
            mp += c->equip.objects[i]->stats.mp;
            atk += c->equip.objects[i]->stats.atk;
            def += c->equip.objects[i]->stats.def;
            mag += c->equip.objects[i]->stats.mag;
            spr += c->equip.objects[i]->stats.spr;
        }
    }

    if (hp < 0)
        hp = 0;
    if (mp < 0)
        mp = 0;
    if (atk < 0)
        atk = 0;
    if (def < 0)
        def = 0;
    if (mag < 0)
        mag = 0;
    if (spr < 0)
        spr = 0;

    statistics_t tmpStats = newStatistics(hp, mp, atk, def, mag, spr);
    printf("statistiche con equipaggiamento:\n");
    printStatistics(&tmpStats);
    printf("\n");
}

void printEquip(character_t c)
{
    printf("\nequipaggiamenti assegnati:\n");
    int i;
    for (i = 0; i < MAX_EQUIP; i++)
    {
        if (c->equip.slots[i])
        {
            printf("%d -> ", i);
            printObject(c->equip.objects[i]);
            printf("\n");
        }
    }
}

void addEquip(character_t c, inventory_t inv, int objIdx)
{
    if (c->equip.count >= MAX_EQUIP)
    {
        printf("[ERRORE:aggiungi equipaggiamento] equipaggiamento al completo\n");
        return;
    }
    int i, freePos;
    for (i = 0; i < MAX_EQUIP; i++)
    {
        if (!c->equip.slots[i])
        {
            freePos = i;
            break;
        }
    }
    c->equip.count++;
    c->equip.slots[freePos] = true;
    object_t *o = getInventoryObject(inv, objIdx);
    c->equip.objects[freePos] = o;
}

void removeEquip(character_t c, int equipIdx)
{
    if (c->equip.count == 0)
    {
        printf("[ERRORE:rimuovi equipaggiamento] nessun equipaggiamento\n");
        return;
    }
    --c->equip.count;
    c->equip.slots[equipIdx] = false;
    c->equip.objects[equipIdx] = NULL;
}

character_t _newCharacter(char *tmpCod, char *tmpName, char *tmpClass, int hp, int mp, int atk, int def, int mag, int spr)
{
    character_t c = malloc(sizeof(struct _character));
    c->cod = strdup(tmpCod);
    c->name = strdup(tmpName);
    c->class = strdup(tmpClass);
    c->stats = newStatistics(hp, mp, atk, def, mag, spr);
    c->equip = _newEquip();
    return c;
}

struct _equip _newEquip()
{
    struct _equip e;
    int i;
    for (i = 0; i < MAX_EQUIP; i++)
    {
        e.slots[i] = false;
        e.objects[i] = NULL;
    }
    return e;
}

void _printCharacterFunction(ll_item_key_t inC)
{
    character_t c = (character_t)inC;
    printCharacter(c);
}

bool _matchCharacterFunction(ll_item_key_t cod, ll_item_t c)
{
    character_t tmpC = (character_t)c;
    return strcmp(tmpC->cod, cod) == 0;
}