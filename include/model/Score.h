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

/**
 * @brief Score module to manage a score 
 * object and store score files
 * 
 */


typedef struct score {
    int mana;
    long time;
    char name[NAME_LENGTH + 1];
} Score;

/**
 * @brief Right scores in a files
 * 
 * @param path the name of the file
 * @param scores the scores
 * @param n number of score to read
 * @return size_t the actual number of scores read
 */
size_t score_read(const char *path, Score *scores, int n);

/**
 * @brief Save scores into a file
 * 
 * @param path the name of the file
 * @param scores the scores
 * @param n number of score to save
 * @return size_t the actual number of saved scores
 */
size_t score_write(const char *path, Score *scores, int n);

/**
 * @brief Compare two scores by mana 
 * 
 * @param s1 
 * @param s2 
 * @return int 
 */
int score_cmp_mana(const void *s1, const void *s2);

/**
 * @brief Compare two scores by time
 * 
 * @param s1 
 * @param s2 
 * @return int 
 */
int score_cmp_time(const void *s1, const void *s2);

/**
 * @brief Get the mana of a score
 * 
 * @param score 
 * @return int 
 */
int score_get_mana(const Score *score);

/**
 * @brief Get the time (as epoch) of a score
 * 
 * @param score 
 * @return long 
 */
long score_get_time(const Score *score);

/**
 * @brief Sprintf the mana of the score
 * 
 * @param score 
 * @param buff 
 */
void score_sprintf_mana(const Score *score, char *buff);

/**
 * @brief Sprintf the time representation (m:s:ss)
 * 
 * @param score 
 * @param buff 
 */
void score_sprintf_time(const Score *score, char *buff);

#endif /* STEALTH_SCORE_H */
