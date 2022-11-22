/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_CHARACTER_H
#define STEALTH_CHARACTER_H

#include <assert.h>
#include "core/geometry/Position.h"
#include "core/Settings.h"
#include "Direction.h"

/* Module that implements the game character and all its functionalities */

typedef struct {
    Position position; /*<!  The coordinates where the player is(not the cell index he is on)  */
    int mana;
    double speed;
    Direction direction; /*<! Need to store the direction to detect a change of direction */
} Character;

/**
 * Init a given character with default values
 * @param new_character
 * @param x
 * @param y
 */
void character_init(Character *new_character, int x, int y);

void character_update_speed(Character *character, Direction direction);

void character_move(Character *character, int x, int y);

#endif /* STEALTH_CHARACTER_H */