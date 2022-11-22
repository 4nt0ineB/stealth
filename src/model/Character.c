
#include "model/Character.h"
#include "core/Settings.h"

#include <assert.h>

void character_init(Character *new_character, int x, int y){
    assert(new_character);
    position_init(&new_character->position, x, y);
    new_character->mana = 0;
    new_character->speed = PLAYER_INIT_SPEED;
    new_character->direction = STILL;
}

void character_update_speed(Character *character, Direction direction){
    assert(character);
    /* Here we have to check if the
     * character has abilities that permits
     * to go over the normal maximum speed
     * */
    if(direction != character->direction){
        character->direction = direction;
        /* The player changed direction, so the speed is reset at init speed */
        character->speed = PLAYER_INIT_SPEED;
    }
    double new_speed = character->speed + PLAYER_INCR_SPEED;
    character->speed = new_speed > PLAYER_MAX_SPEED ? PLAYER_MAX_SPEED : new_speed;
}

void character_move(Character *character, int x, int y){
    assert(character);
    character->position.x = x;
    character->position.y = y;
}