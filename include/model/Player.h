/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_PLAYER_H
#define STEALTH_PLAYER_H

#include <assert.h>
#include "core/geometry/Position.h"
#include "core/Settings.h"
#include "Direction.h"
#include "Skill.h"

/* Module that implements the game character and all its functionalities */

typedef struct {
    int mana;
    Direction direction; /*<! Need to store the direction to detect a change of direction */
    double speed;
    Position position; /*<!  The coordinates where the entity is (not the cell index it is on)  */
    Skill skills[SPEED + 1];
} Player;

/**
 * Init a given character with default values
 * @param player
 * @param x
 * @param y
 */
void player_init(Player *player, int x, int y);

void player_update_speed(Player *player, Direction direction);

/**
 * Deactivate skills that can't be used as a result
 * of the player lack of mana
 * @param player
 */
void player_update_skills_state(Player *player);

/**
 * Activate a player skill
 * @param player
 * @param type
 */
void player_activate_skill(Player *player, SkillType type);

/**
 * Update the mana of the player and return the amount consumed
 * @param player
 * @return
 */
int player_consume_mana(Player *player);

#endif /* STEALTH_PLAYER_H */