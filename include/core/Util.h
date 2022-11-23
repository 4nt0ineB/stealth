/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */


#ifndef STEALTH_UTIL_H
#define STEALTH_UTIL_H

#define SQRT2 1.414213562

/**
 * @file Util.h
 * @brief Useful functions used across the project
 *
 */

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define CLAMP(min, val, max) (MAX(min, MIN(max, val)))


/**
 * Return a random int between two values
 * @param a
 * @param b
 * @return
 */
int int_rand(int a, int b);

/**
 * Return a random float between two values
 * @param min
 * @param max
 * @return
 */
float float_rand(float min, float max);

/**
 * Compute a modulo safely
 * @param a
 * @param b
 * @return
 */
int modulo(int a, int b);

/**
 * Calculate the greatest common multiplier of two numbers,
 * inferior to a given border (inclusive)
 * @param a
 * @param b
 * @param max upper bound (incluse)
 * @return
 */
int greatest_common_multiplier_less_than(int a, int b, int max);



#endif /* STEALTH_UTIL_H */
