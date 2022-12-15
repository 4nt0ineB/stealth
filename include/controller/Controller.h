/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_CONTROLLER_H
#define STEALTH_CONTROLLER_H

#include "core/Settings.h"
#include "model/Room.h"

typedef struct game_data {
    Player player;
    Relic relics[RELICS_NUMBER];
    Guard guards[GUARD_NUMBER];
    Room room;
}GameData;



int slealth_controller();

void controller_init(GameData *data);

/**
* Places RELICS_NUMBER of relics in the room randomly not at spawn and not in a wall
* @param room
*
*/
static void controller_init_relics(GameData *data);

/**
 * Moves the player according to a given direction
 * @param room
 * @param direction
 */
void controller_move_player(GameData *data, Direction direction);

/**
 * Moves the guards of the room
 * @param room
 */
void controller_move_guards(GameData *data);


/**
 * Put guards in panic mode
 * @param data
 */
void controller_check_guard_panic(GameData *data);

/**
 * Check if any guards detected the player
 * @param data
 * @return 1 if caught, otherwise 0
 */
int controller_check_guards_find_player(GameData *data);

/**
 * Get the mana or the relics if the player
 * is on one of these tiles
 * @param data
 */
void controller_check_player(GameData *data);

/**
 * Return the number of stolen relics
 * @param data
 * @return
 */
int controller_stolen_relic_count(const GameData *data);

#endif /* STEALTH_CONTROLLER_H */ 