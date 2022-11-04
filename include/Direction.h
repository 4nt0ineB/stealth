/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
*/

#ifndef STEALTH_DIRECTION_H
#define STEALTH_DIRECTION_H
#include "Controller.h"

typedef enum {
    RIGHT, 
    LEFT,
    UP,
    DOWN
} Direction;

Direction new_dir();

#endif