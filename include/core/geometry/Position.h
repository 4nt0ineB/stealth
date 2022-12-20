/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_POSITION_H
#define STEALTH_POSITION_H

/**
 * @brief Coordinates implantation
 *
 */
typedef struct position {
    double x, y;
} Position;

typedef Position Vector;

/**
 * Print the position on stdout
 * @param position
 */
void position_print(Position *position);

/**
 * Assign given coordinates into the position
 * @param position
 */
void position_init(Position *position, int x, int y);

/**
 * Calculate the distance between two positions
 * @param a
 * @param b
 * @return
 */
double position_dist(const Position *a, const Position *b);

/**
 * Check if two position are equal
 * @param a
 * @param b
 * @return
 */
int position_eq(const Position *a, const Position *b);

/**
 * Subtract two positions
 * @param a
 * @param b
 * @param result
 */
void position_sub(const Position *a, const Position *b, Position *result);

/**
 * Interpolate x with a known y on segment
 * @param a
 * @param b
 * @param interpolated position with known y
 * @return
 */
void position_interpolate_with_x(const Position *a, const Position *b, Position *to_find);

/**
 * Interpolate y with a known x on segment
 * @param a
 * @param b
 * @param to_find position with known x
 * @return
 */
void position_interpolate_with_y(const Position *a, const Position *b, Position *to_find);

/**
 * Calculate the magnitude of a vector
 * @param v
 * @return
 */
double vector_mag(const Vector *v);

#endif /* STEALTH_POSITION_H */
