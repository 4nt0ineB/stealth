/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
*/

#ifndef STEALTH_DIRECTION_H
#define STEALTH_DIRECTION_H

/**
 * The cardinal directions,
 * and STILL when motionless
 */
typedef enum {
    STILL = 0,
    N,
    NE,
    E,
    SE,
    S,
    SO,
    O,
    NO
} Direction;

/**
 * factor to apply to a vector depending on Direction
 */
extern const double direction_factor[9][2];

Direction direction_previous(Direction direction);

Direction direction_next(Direction direction);

#endif