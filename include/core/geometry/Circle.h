/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_CIRCLE_H
#define STEALTH_CIRCLE_H

#include "Position.h"

/**
 * @brief Circle implantation
 *
 */
typedef struct circle {
    int r;
    Position origin;
} Circle;

#endif /* STEALTH_CIRCLE_H */
