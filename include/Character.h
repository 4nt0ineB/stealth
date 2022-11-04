/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_CHARACTER_H
#define STEALTH_CHARACTER_H
#include "Controller.h"

/* Module that implements the game character and all its functionalities */

typedef struct {
    int x; /* MAYBE FLOATS BECAUSE OF SPEED ???*/
    int y;
    int mana;
    int speed;
} Character;

Character init_character();

#endif /* STEALTH_CHARACTER_H */