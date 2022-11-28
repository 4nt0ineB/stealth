/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_SKILL_H
#define STEALTH_SKILL_H

typedef enum {
    INVISIBILITY = 0
    , SPEED
} SkillType;

typedef struct skill {
    int activated;
    SkillType type;
    int mana_consumption;
} Skill;

void skill_init(Skill *skill, SkillType type);

#endif /* STEALTH_SKILL_H */
