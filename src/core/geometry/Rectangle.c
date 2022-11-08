/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include "core/geometry/Rectangle.h"
#include <stdio.h>


void rectangle_print(Rectangle *rectangle){
    printf("Rectangle [");
    position_print(&rectangle->origin);
    printf(", w: %d, h: %d]"
           , rectangle->w
           , rectangle->h);
}