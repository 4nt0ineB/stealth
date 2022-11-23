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

void init_room(Room *new_room);

void print_room(Room room);


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

void room_move_guards(Room *room);

#endif /* STEALTH_ROOM_H */