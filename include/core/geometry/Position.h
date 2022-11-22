/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_POSITION_H
#define STEALTH_POSITION_H

/**
 * @brief Coordinates implantation
 *
 */
typedef struct position {
    double x, y;
} Position;

typedef Position Vector;

/**
 * Print the position on stdout
 * @param position
 */
void position_print(Position *position);

/**
 * Assign given coordinates into the position
 * @param position
 */
void position_init(Position *position, int x, int y);

#endif /* STEALTH_POSITION_H */
