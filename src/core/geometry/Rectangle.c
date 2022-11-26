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

void rectangle_with_padding(const Rectangle *rectangle, double paddingx, double paddingy, Rectangle *result){
    paddingx = rectangle->w * paddingx;
    paddingy = rectangle->h * paddingy;
    result->origin.x = rectangle->origin.x + paddingx;
    result->origin.y = rectangle->origin.y + paddingy;
    result->w = rectangle->w - paddingx * 2;
    result->h = rectangle->h - paddingy * 2;
}