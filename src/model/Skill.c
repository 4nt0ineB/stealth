/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include "model/Skill.h"
#include "core/Settings.h"

void skill_init(Skill *skill, SkillType type){
    assert(type >= 0 && type < SKILL_NUMBER);
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