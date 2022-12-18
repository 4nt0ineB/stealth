/*
 *   This file is part of Stealth game
 *    Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include "model/Skill.h"
#include "core/Settings.h"

void skill_check_type(SkillType type){
    assert(type >= 0 && type < SKILL_NUMBER);
}

void skill_init(Skill *skill, SkillType type){
    skill_check_type(type);
    skill->type = type;
    switch (type) {
        case INVISIBILITY:
            skill->mana_consumption = 1;
            break;
        case SPEED:
            skill->mana_consumption = 2;
        default:
            break;
    }
}

int skill_mana_consumption(const Skill *skill){
    assert(skill);
    return skill->mana_consumption;
}

int skill_is_activated(const Skill *skill){
    assert(skill);
    return skill->activated;
}

void skill_activate(Skill *skill){
    assert(skill);
    skill->activated = 1;
}
void skill_deactivate(Skill *skill){
    assert(skill);
    skill->activated = 0;
}