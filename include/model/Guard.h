/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_GUARD_H
#define STEALTH_GUARD_H

#include "core/Settings.h"
#include "core/Util.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "Direction.h"
#include "core/geometry/Position.h"

/**
 * Module that implements the guards of the game and all their functionalities
 */

typedef struct {
    int panic_mode;
    Direction direction; /*<! Need to store the direction to detect a change of direction */
    double speed;
    Position position; /*<!  The coordinates where the entity is (not the cell index it is on)  */
    int panic_count; /* to count panick mode time passed */
} Guard;

/**
 * Function that init a new guard at his inital values
 * @param guard
 * @param x
 * @param y
 */
void guard_init(Guard *guard, int x, int y);

/**
 * Make the guard go into panick mode
 * @param guard
 */
void guard_panick(Guard *guard);

/**
 * Make the guard drop his panick mode
 * @param guard
 */
void guard_unpanick(Guard *guard);

Direction guard_update_direction(Guard *guard);

void guard_update_speed(Guard *guard);

double guard_view_range(const Guard *guard);

void guard_reset_panick_count(Guard *guard);

void guard_update_panick_count(Guard *guard);


#endif /* STEALTH_GUARD_H */