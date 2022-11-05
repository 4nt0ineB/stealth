
#include "Character.h"

void init_character(Character *new_character, int x, int y){
    new_character->x = x;
    new_character->y = y;
    new_character->mana = 0;
    new_character->speed = SPEED / 10;
}


