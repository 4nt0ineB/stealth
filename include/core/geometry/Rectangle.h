/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_RECTANGLE_H
#define STEALTH_RECTANGLE_H

#include "Position.h"

typedef struct rectangle {
    int w, h;
    Position origin;
} Rectangle;

/**
 * Print rectangle data on stdout
 * @param rectangle
 */
void rectangle_print(Rectangle *rectangle);

#endif /* STEALTH_RECTANGLE_H */
