
#include "Character.h"

Character init_character(int x, int y){
    Character new_character;
    new_character.x = x;
    new_character.y = y;
    new_character.mana = 0;
    new_character.speed = SPEED / 10;
    return new_character;
}


