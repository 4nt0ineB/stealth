/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "core/geometry/Position.h"


void position_print(Position *position){
    printf("(%d, %d)", position->x, position->y);
}