#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "statistics.h"
#include "equipment.h"
#include "inventory.h"

#define STR_BUFF_SIZE 51

typedef struct _character *character_t;

character_t newCharacter(char *cod, char *name, char *c, int hp, int mp, int atk, int def, int mag, int spr);
void freeCharacter(character_t c);
void printCharacter(character_t c);
void addCharacterEquipment(character_t c, inventory_t inv, int itemIdx);
void removeCharacterEquipment(character_t c, int equipItemIdx);
bool matchCharacter(character_t c, char *cod);
void printCharacterEquipment(character_t c);

#endif
