
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
    double max_speed = skill_is_activated(&player->skills[SPEED]) ? SKILL_MAX_SPEED : PLAYER_MAX_SPEED;
    double new_speed = player->speed + PLAYER_INCR_SPEED;
    player->speed = MIN(new_speed, max_speed);
}

void player_update_skills_state(Player *player){
    int i;
    assert(player);
    for(i = 0; i < SKILL_NUMBER; i++){
        if(player->skills[i].activated
        && player->mana < skill_mana_consumption(&player->skills[i])){
            skill_deactivate(&player->skills[i]);
        }
    }
}

void player_activate_skill(Player *player, SkillType type){
    skill_check_type(type);
    skill_activate(&player->skills[type]);
}

void player_deactivate_all_skills(Player *player){
    int i;
    for(i = INVISIBILITY; i <= SPEED; i++){
        skill_deactivate(player_skill(player, i));
    }
}

int player_consume_mana(Player *player){
    int i, count, mana;
    for(i = count = 0; i < SKILL_NUMBER; i++){
        mana = skill_mana_consumption(&player->skills[i]);
        if(skill_is_activated(&player->skills[i])
           && player->mana >= mana){
            count += mana;
        }
    }
    player->mana -= count;
    return count;
}

Skill * player_skill(const Player *player, SkillType type){
    skill_check_type(type);
    return &player->skills[type];
}
