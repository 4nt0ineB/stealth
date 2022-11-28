/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_RELIC_H
#define STEALTH_RELIC_H

#include "core/geometry/Position.h"
#include "assert.h"

/**
 * Module that implements the relics of the game and all their functionalities
 * */

typedef struct {
    Position position; /*<! Pos of the relic int the room */
    int stolen; /*<! Says if the relic is still on the room or not */
    int noticed; /*<! Has */
} Relic;

void init_relic(Relic *relic, Position pos);

void take_relic(Relic *relic);


#endif /* STEALTH_RELIC_H */