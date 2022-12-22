/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_TILE_H
#define STEALTH_TILE_H
#include <stdio.h>

typedef enum {
    EMPTY,
    WALL,
    MANA,
    RELIC
} TileType;

typedef struct {
    TileType type;
} Tile;

int tile_get_type(const Tile *tile);
void tile_set_type(Tile *tile, TileType type);
#endif