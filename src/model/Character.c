
#include "model/Character.h"
#include "core/Settings.h"

void init_character(Character *new_character, int x, int y){
    new_character->x = x;
    new_character->y = y;
    new_character->mana = 0;
    new_character->speed = SPS / 10;
}


