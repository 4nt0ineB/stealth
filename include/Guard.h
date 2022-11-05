/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_GUARD_H
#define STEALTH_GUARD_H
#include "Controller.h"
#include "Direction.h"

/* Module that implements the guards of the game and all their functionalities */


typedef struct {
    int x; /* MAYBE FLOATS BECAUSE OF SPEED ???*/
    int y;
    int speed;
    int panick_mode;
    Direction direction;    
} Guard;

/* Function that init a new guard at his inital values*/
void init_guard(Guard *guard, int x, int y);

/* Make the guard go into panick mode */
void make_panick(Guard *guard);

/* Make the guard drop his panick mode */
void unpanick(Guard *guard);

/* Function that makes the guard move depending his mode of moving (panick or not)*/
void guard_move(Guard *guard);

#endif /* STEALTH_GUARD_H */