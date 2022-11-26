/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_ROOM_H
#define STEALTH_ROOM_H
#include "core/Settings.h"
#include "Tile.h"
#include "Character.h"
#include "Guard.h"
#include <stdio.h>
#include <stdlib.h>

/* Module that will implement the board game */

#define MINSIDE 9

typedef struct {
    Tile tiles[ROOM_HEIGHT][ROOM_WIDTH];
    Character player;
    Guard guards[GUARD_NUMBER];
} Room;

void room_init(Room *room);

void room_print(Room room);

/**
 * Test if the entity is colliding a wall of the room
 * and respond to the collision by moving the entity to
 * the appropriate position.
 * @param room
 * @param position
 * @return 1 if entity has collided 0 if not
 */
int room_resolve_collision(Room *room, Position *position);

/**
 * Moves the player according to a given direction
 * @param room
 * @param direction
 */
void room_move_player(Room *room, Direction direction);

/**
 * Moves the guards of the room
 * @param room
 */
void room_move_guards(Room *room);

/**
 * Check if a tile of a given type is on the
 * segment formed by to position
 * @param room
 * @param p1
 * @param p2
 * @param tile_type
 * @return 1 if true, otherwise 0
 */
int room_tile_between(const Room *room, const Position *p1, const Position *p2, TileType tile_type);


void room_check_guard_panic(Room *room);

void room_check_player(Room *room);

#endif /* STEALTH_ROOM_H */