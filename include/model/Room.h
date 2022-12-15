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
#include "Player.h"
#include "Guard.h"
#include "Relic.h"
#include <stdio.h>
#include <stdlib.h>

/* Module that will implement the board game */

#define MINSIDE 9

typedef struct {
    Tile tiles[ROOM_HEIGHT][ROOM_WIDTH];
} Room;

void room_init(Room *room);

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
 * Check if a tile of a given type is on the
 * segment formed by to position
 * @param room
 * @param p1
 * @param p2
 * @param tile_type
 * @return 1 if true, otherwise 0
 */
int room_tile_between(const Room *room, const Position *p1, const Position *p2, TileType tile_type);

/**
 * Set a given amount of mana tiles
 * in the room
 * @param room
 * @param amount
 */
void room_add_mana(Room *room, int amount);



int room_get_tile_type(const Room *room, int i, int j);

Tile * room_get_tile(Room *room, int i, int j);

/**
 * Get a random tile index of a given type
 * @param room
 * @param tile_type the type of tile to get
 * @return 1 if indexes found, otherwise 0
 */
int room_random_position(const Room *room, TileType tile_type, int *res_x, int *res_y);

#endif /* STEALTH_ROOM_H */