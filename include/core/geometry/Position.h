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
    int x, y;
} Position;

/**
 * Print the position on stdout
 * @param position
 */
void position_print(Position *position);

#endif /* STEALTH_POSITION_H */
