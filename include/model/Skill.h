/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_SKILL_H
#define STEALTH_SKILL_H

/**
 * @brief Skill module to manage a skill 
 */

typedef enum {
    INVISIBILITY = 0
    , SPEED
} SkillType;

typedef struct skill {
    int activated; /*<! 1 if activated, otherwise false */
    SkillType type; /*<! the type of the skill */
    int mana_consumption; /*<! The mana it consume each frame */
} Skill;

/**
 * @brief Check if a given Skill type is valid
 * 
 * @param type 
 */
void skill_check_type(SkillType type);

/**
 * @brief Init a skill for a given skill type
 * 
 * @param skill 
 * @param type 
 */
void skill_init(Skill *skill, SkillType type);

/**
 * @brief Get the mana consumption by frame of a skill
 * 
 * @param skill 
 * @return int 
 */
int skill_mana_consumption(const Skill *skill);

/**
 * @brief Check if a skill is active
 * 
 * @param skill 
 * @return int 
 */
int skill_is_activated(const Skill *skill);

/**
 * @brief Activate a skill
 * 
 * @param skill 
 */
void skill_activate(Skill *skill);

/**
 * @brief Deactivate a skill
 * 
 * @param skill 
 */
void skill_deactivate(Skill *skill);

#endif /* STEALTH_SKILL_H */
