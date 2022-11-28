
#include "model/Player.h"
#include "core/Settings.h"
#include "core/Util.h"

#include <assert.h>

void player_init(Player *player, int x, int y){
    int i;
    assert(player);
    position_init(&player->position, x, y);
    player->mana = 0;
    player->speed = PLAYER_INIT_SPEED;
    player->direction = STILL;
    for(i = 0; i < SKILL_NUMBER; i++){
        skill_init(&player->skills[i], i);
    }
}

void player_update_speed(Player *player, Direction direction){
    assert(player);

    /* if the players entirely reverse his direction, his speed is reinitialized */
    if(player->direction != direction
    && player->direction != direction_previous(direction)
    && player->direction != direction_next(direction)){
        player->direction = direction;
        /* The player changed direction, so the speed is reset at init speed */
        player->speed = PLAYER_INIT_SPEED;
    }
    /* Here we have to check if the
     * character has abilities that permits
     * to go over the normal maximum speed
     * */
    double max_speed = player->skills[SPEED].activated ? SKILL_MAX_SPEED : PLAYER_MAX_SPEED;
    double new_speed = player->speed + PLAYER_INCR_SPEED;
    player->speed = MIN(new_speed, max_speed);
}

void player_update_skills_state(Player *player){
    int i;
    assert(player);
    for(i = 0; i < SKILL_NUMBER; i++){
        if(player->skills[i].activated
        && player->mana < player->skills[i].mana_consumption){
            player->skills[i].activated = 0;
        }
    }
}

void player_activate_skill(Player *player, SkillType type){
    assert(type >= 0 && type < SKILL_NUMBER);
    player->skills[type].activated = 1;
}

int player_consume_mana(Player *player){
    int i, count;
    for(i = count = 0; i < SKILL_NUMBER; i++){
        if(player->skills[i].activated
           && player->mana >= player->skills[i].mana_consumption){
            count += player->skills[i].mana_consumption;
        }
    }
    player->mana -= count;
    return count;
}