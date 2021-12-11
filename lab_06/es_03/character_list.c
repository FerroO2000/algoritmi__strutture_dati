#include "character_list.h"

struct _characterList
{
    linked_list_t list;
    size_t count;
};

void _printCharacterFunction(ll_item_key_t inC);
bool _matchCharacterFunction(ll_item_key_t cod, ll_item_t c);
void _freeCharacterFunction(ll_item_t item);

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
    cl->list = newLinkedList(_freeCharacterFunction, _matchCharacterFunction, _printCharacterFunction);

    while (fgets(lineBuff, FILE_LINE_BUFF_SIZE, fp))
    {
        sscanf(lineBuff, "%s %s %s %d %d %d %d %d %d\n", tmpCod, tmpName, tmpClass, &hp, &mp, &atk, &def, &mag, &spr);
        pushItem(cl->list, newCharacter(tmpCod, tmpName, tmpClass, hp, mp, atk, def, mag, spr));
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
    printLinkedList(cl->list);
}

void addCharacter(character_list_t cl, char *cod, char *name, char *class, int hp, int mp, int atk, int def, int mag, int spr)
{
    pushItem(cl->list, newCharacter(cod, name, class, hp, mp, atk, def, mag, spr));
    cl->count++;
}

void removeCharacter(character_list_t cl, char *cod)
{
    ll_item_t remC = removeItem(cl->list, cod);
    if (remC == NULL)
    {
        return;
    }
    --cl->count;
    _freeCharacterFunction(remC);
}

character_t getCharacter(character_list_t cl, char *cod)
{
    character_t c = (character_t)getItem(cl->list, cod);
    return c;
}

void _printCharacterFunction(ll_item_key_t c)
{
    printCharacter(c);
}

bool _matchCharacterFunction(ll_item_key_t cod, ll_item_t c)
{
    return matchCharacter(c, cod);
}

void _freeCharacterFunction(ll_item_t c)
{
    freeCharacter(c);
}