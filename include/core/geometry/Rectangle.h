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

/**
 * Set the values of the Rectangle
 * @param rectangle
 * @param x
 * @param y
 * @param w
 * @param h
 */
void rectangle_init(Rectangle  *rectangle, int x, int y, int w, int h);

/**
 * For a given rectangle fill the values of an other one as
 * a centered inner rectangle with padding.
 * @param rectangle
 * @param paddingx
 * @param paddingy
 * @param result
 */
void rectangle_with_padding(const Rectangle *rectangle, double paddingx, double paddingy, Rectangle *result);

#endif /* STEALTH_RECTANGLE_H */
