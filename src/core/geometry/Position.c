/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "core/geometry/Position.h"

#include <assert.h>

void position_print(Position *position){
    printf("(%d, %d)", position->x, position->y);
}

void position_init(Position *position, int x, int y){
    assert(position);
    position->x = x;
    position->y = y;
}
