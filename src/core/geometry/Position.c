/*
 *   This file is part of Stealth game
 *    Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "core/geometry/Position.h"

#include <assert.h>
#include <math.h>

void position_print(Position *position){
    printf("(%f, %f)", position->x, position->y);
}

void position_init(Position *position, int x, int y){
    assert(position);
    position->x = x;
    position->y = y;
}

double position_dist(const Position *a, const Position *b){
    return sqrt((b->x - a->x) * (b->x - a->x)
    + (b->y - a->y) * (b->y - a->y));
}

void position_sub(const Position *a, const Position *b, Position *result){
    assert(a && b && result);
    result->x = a->x - b->x;
    result->y = a->y - b->y;
}

double vector_mag(const Vector *v){
    assert(v);
    return sqrt(v->x * v->x + v->y * v->y);
}

void position_interpolate_with_x(const Position *a, const Position *b, Position *to_find){
    to_find->x = a->x + ((b->x - a->x) / (b->y - a->y)) * (to_find->y - a->y);
}

void position_interpolate_with_y(const Position *a, const Position *b, Position *to_find){
    to_find->y = a->y + ((b->y - a->y) / (b->x - a->x)) * (to_find->x - a->x);
}

int is_same_position(const Position *a, const Position *b){
    return (a->x == b->x && a->y == b->y);
}
