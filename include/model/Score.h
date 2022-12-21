/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_SCORE_H
#define STEALTH_SCORE_H

#include "core/Settings.h"
#include <stdio.h>

typedef struct score {
    int mana;
    long time;
    char name[NAME_LENGTH + 1];
} Score;

size_t score_read(const char *path, Score *scores, int n);

size_t score_write(const char *path, Score *scores, int n);

int score_cmp_mana(const void *s1, const void *s2);
int score_cmp_time(const void *s1, const void *s2);
int score_get_mana(const Score *score);
long score_get_time(const Score *score);

void score_sprintf_mana(const Score *score, char *buff);
void score_sprintf_time(const Score *score, char *buff);

#endif /* STEALTH_SCORE_H */
