/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_CONTROLLER_H
#define STEALTH_CONTROLLER_H

#include "GameData.h"
#include "view/View.h"
#include <MLV/MLV_all.h>

/**
 * Entry of the program loop
 * @return
 */
int controller_run();

/**
 *
 * @param view
 * @param data
 * @return 1 if want to play, 0 otherwise
 */
int controller_menu(View *view, GameData *data);

/**
 * The game loop
 * @param view
 * @param data
 * @return
 */
int controller_game_loop(View *view, GameData *data);

/**
 * End game screen
 * @param view
 * @param data
 * @param win
 * @return 1 if want to retry, otherwise 0
 */
int controller_end_game(View *view, GameData *data, int win);

/**
 * In the game data
 * @param data
 */
void controller_init(GameData *data);

/**
 * Init audio files of the game
 * @param data
 * @return 1 if error, otherwise 0
 */
int controller_init_audio(GameData *data);

/**
* Places RELICS_NUMBER of relics in the room randomly not at spawn and not in a wall
* @param room
*
*/
void controller_init_relics(GameData *data);

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
 * @return 1 if guard are panicking, otherwise 0
 */
int controller_check_guard_panic(GameData *data);

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

/**
 * Update the timer of the game
 * @param data
 */
void controller_update_time(GameData *data);

/**
 * Moves the entity by a given Velocity (speed and direction)
 * @param position
 * @param speed
 * @param direction
 */
void entity_move(Position *position, double speed, Direction direction);

Direction get_direction_from_keyboard();

/**
 * Draw all the elements of the game
 * @param data
 * @param view
 */
void controller_update_view(GameData *data, View *view);

/**
 * Test if the player won
 * @param data
 * @return
 */
int controller_win(const GameData *data);

/**
 * Save the score if in the top
 * @param view
 * @param data
 */
void controller_save_score(View *view, GameData *data);

/**
 * Update the panic mode
 * of the guards if a missing relic is noticed
 * @param room
 * @param guard
 * @param relic
 * @return
 */
int controller_guard_sees_missing_relic(const Room *room, const Guard *guard, const Relic *relic);

/**
 * Find a player where coordinates are in the button box
 * @param buttons
 * @param n
 * @param x
 * @param y
 * @return a pointer to the button or null
 */
Button * controller_pos_over_button(const View *view, Button *buttons, int n, int x, int y);

#endif /* STEALTH_CONTROLLER_H */ 