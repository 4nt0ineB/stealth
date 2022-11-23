
#include "model/Character.h"
#include "core/Settings.h"

#include <assert.h>

void character_init(Character *character, int x, int y){
    assert(character);
    position_init(&character->position, x, y);
    character->mana = 0;
    character->speed = PLAYER_INIT_SPEED;
    character->direction = STILL;
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
