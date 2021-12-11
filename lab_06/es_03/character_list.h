#ifndef CHARACTER_LIST_H
#define CHARACTER_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"
#include "statistics.h"
#include "inventory.h"
#include "character.h"

#define STR_BUFF_SIZE 51
#define FILE_LINE_BUFF_SIZE 1024

typedef struct _characterList *character_list_t;

character_list_t newCharacterListFromFile(char *fileName);
void addCharacter(character_list_t cl, char *cod, char *name, char *c, int hp, int mp, int atk, int def, int mag, int spr);
void removeCharacter(character_list_t cl, char *cod);
void printCharacterList(character_list_t cl);
character_t getCharacter(character_list_t cl, char *cod);
void freeCharacterList(character_list_t cl);

#endif
