/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_TILE_H
#define STEALTH_TILE_H
#include <stdio.h>

/**
 * @brief Tile module to manage a tile 
 */

typedef enum {
    EMPTY,
    WALL,
    MANA,
    RELIC
} TileType;

typedef struct {
    TileType type;
} Tile;

/**
 * @brief Get the type of a tile
 * 
 * @param tile 
 * @return int 
 */
int tile_get_type(const Tile *tile);

/**
 * @brief Set the type of tile
 * 
 * @param tile 
 * @param type 
 */
void tile_set_type(Tile *tile, TileType type);

#endif