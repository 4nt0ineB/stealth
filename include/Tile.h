/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_TILE_H
#define STEALTH_TILE_H

typedef enum {
    WALL,
    TILE,
    RELIC
} TileType;

typedef struct {
    TileType tile_type;
    int mana;
    int caught;
} Tile;

Tile init_tile(TileType tile_type);

#endif