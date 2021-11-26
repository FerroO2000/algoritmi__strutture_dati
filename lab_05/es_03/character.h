#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include "statistics.h"
#include "inventory.h"

#define STR_BUFF_SIZE 51

typedef struct _characterList *character_list_t;
typedef struct _character *character_t;

character_list_t newCharacterListFromFile(char *fileName);
void addCharacter(character_list_t cl, char *cod, char *name, char *class, int hp, int mp, int atk, int def, int mag, int spr);
void removeCharacter(character_list_t cl, char *cod);
void printCharacterList(character_list_t cl);
character_t getCharacter(character_list_t cl, char *cod);
void printCharacter(character_t c);
void addEquip(character_t c, inventory_t inv, int objIdx);
void removeEquip(character_t c, int equipIdx);
void printEquip(character_t c);
void freeCharacterList(character_list_t cl);

#endif
