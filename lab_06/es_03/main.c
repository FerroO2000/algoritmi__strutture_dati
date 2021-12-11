#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "character.h"
#include "inventory.h"
#include "character_list.h"

/*

    !!! IMPORTANTE !!!

    descrizione moduli:

    -   "inventory.h" = "invArray" 
    -   "inventory_item.h" = "inv"
    -   "equipment.h" = "equipArray"
    -   "character.h" = "pg"
    -   "character_list.h" = "pgList"


    -   "statistics.h" = gestisce le statistiche dei personaggi e oggetti
    -   "linked_list.h" = astrazione semi-generica delle liste caso mai dovessero servire in seguito

*/

#define CMD_COUNT 7

typedef enum
{
    LIST_CHARACTERS,
    ADD_CHARACTER,
    REMOVE_CHARACTER,
    ADD_CHARACTER_EQUIP,
    REMOVE_CHARACTER_EQUIP,
    PRINT_CHARACTER,
    EXIT,
} cmd_e;

cmd_e readCmd();
void addCharacterCmd(character_list_t cl);
void removeCharacterCmd(character_list_t cl);
void addCharacterEquipCmd(character_list_t cl, inventory_t inv);
void removeCharacterEquipCmd(character_list_t cl, inventory_t inv);
void printCharacterCmd(character_list_t cl);

static char *cmdNames[CMD_COUNT] = {"stampa tutti i personaggi", "aggiungi personaggio", "rimuovi personaggio",
                                    "aggiungi equipaggiamento", "rimuovi equipaggiamento", "stampa personaggio", "esci"};

int main()
{
    character_list_t cl = newCharacterListFromFile("pg.txt");
    inventory_t inv = newInventoryFromFile("inventario.txt");

    bool stop = false;
    cmd_e cmd;
    while (!stop)
    {
        cmd = readCmd();
        switch (cmd)
        {
        case LIST_CHARACTERS:
            printCharacterList(cl);
            break;

        case ADD_CHARACTER:
            addCharacterCmd(cl);
            break;

        case REMOVE_CHARACTER:
            removeCharacterCmd(cl);
            break;

        case ADD_CHARACTER_EQUIP:
            addCharacterEquipCmd(cl, inv);
            break;

        case REMOVE_CHARACTER_EQUIP:
            removeCharacterEquipCmd(cl, inv);
            break;

        case PRINT_CHARACTER:
            printCharacterCmd(cl);
            break;

        case EXIT:
            printf("esco dal programma ...");
            stop = true;
            break;

        default:
            break;
        }
    }

    freeCharacterList(cl);
    freeInventory(inv);

    return 0;
}

cmd_e readCmd()
{
    printf("\n\n#COMANDI:\n");
    int i;
    for (i = 0; i < CMD_COUNT; i++)
    {
        printf("%d -> %s\n", i, cmdNames[i]);
    }
    printf("comando (numero): ");
    int cmdNum;
    scanf("%d", &cmdNum);
    if (cmdNum >= CMD_COUNT)
    {
        return EXIT;
    }
    printf("\n");
    return (cmd_e)cmdNum;
}

void addCharacterCmd(character_list_t cl)
{
    char tmpCod[STR_BUFF_SIZE], tmpName[STR_BUFF_SIZE], tmpClass[STR_BUFF_SIZE];
    int hp, mp, atk, def, mag, spr;
    printf("codice: ");
    scanf("%s", tmpCod);
    printf("nome: ");
    scanf("%s", tmpName);
    printf("classe: ");
    scanf("%s", tmpClass);
    printf("hp: ");
    scanf("%d", &hp);
    printf("mp: ");
    scanf("%d", &mp);
    printf("atk: ");
    scanf("%d", &atk);
    printf("def: ");
    scanf("%d", &def);
    printf("mag: ");
    scanf("%d", &mag);
    printf("spr: ");
    scanf("%d", &spr);
    addCharacter(cl, tmpCod, tmpName, tmpClass, hp, mp, atk, def, mag, spr);
}

void removeCharacterCmd(character_list_t cl)
{
    char tmpCod[STR_BUFF_SIZE];
    printf("codice personaggio: ");
    scanf("%s", tmpCod);
    removeCharacter(cl, tmpCod);
}

void addCharacterEquipCmd(character_list_t cl, inventory_t inv)
{
    char tmpCod[STR_BUFF_SIZE];
    printf("codice personaggio: ");
    scanf("%s", tmpCod);
    character_t c = getCharacter(cl, tmpCod);
    if (c == NULL)
    {
        printf("codice non trovato\n");
        return;
    }
    printf("\nequipaggiamenti disponibili:\n");
    printInventory(inv);
    int equipNum;
    printf("seleziona equipaggiamento (numero): ");
    scanf("%d", &equipNum);
    addCharacterEquipment(c, inv, equipNum);
}

void removeCharacterEquipCmd(character_list_t cl, inventory_t inv)
{
    char tmpCod[STR_BUFF_SIZE];
    printf("codice personaggio: ");
    scanf("%s", tmpCod);
    character_t c = getCharacter(cl, tmpCod);
    if (c == NULL)
    {
        printf("codice non trovato\n");
        return;
    }
    printCharacterEquipment(c);
    int equipNum;
    printf("seleziona equipaggiamento (numero): ");
    scanf("%d", &equipNum);
    removeCharacterEquipment(c, equipNum);
}

void printCharacterCmd(character_list_t cl)
{
    char tmpCod[STR_BUFF_SIZE];
    printf("codice personaggio: ");
    scanf("%s", tmpCod);
    character_t c = getCharacter(cl, tmpCod);
    if (c == NULL)
    {
        printf("codice non trovato\n");
        return;
    }
    printCharacter(c);
}