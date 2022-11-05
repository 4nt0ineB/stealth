/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_ROOM_H
#define STEALTH_ROOM_H
#include "Tile.h"
#include "Character.h"
#include "Guard.h"
#include <stdio.h>
#include <stdlib.h>

/* Module that will implement the board game */

#define ROOM_WIDTH 45
#define ROOM_HEIGHT 45

#define MINSIDE 9

#define GUARD_NUMBER 5

typedef struct {
    Tile room[ROOM_HEIGHT][ROOM_WIDTH];
    Character player;
    Guard guards[GUARD_NUMBER];
} Room;

void init_room(Room *new_room);

void print_room(Room room);

#endif /* STEALTH_ROOM_H */