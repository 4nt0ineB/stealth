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
    Tile room[ROOM_HEIGHT][ROOM_WIDTH];
    Character player;
    Guard guards[GUARD_NUMBER];
} Room;

void init_room(Room *new_room);

void print_room(Room room);

/**
 * Moves the player according to a given direction
 * @param room
 * @param direction
 */
void room_move_player(Room *room, Direction direction);

#endif /* STEALTH_ROOM_H */