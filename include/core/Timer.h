/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_TIMER_H
#define STEALTH_TIMER_H

/**
 * @file Settings.h
 * @brief some globals and macros needed for the game
 *
 */

#include "core/Util.h"
#include <sys/time.h>
#include <stdio.h>

typedef struct timer {
    unsigned int state: 1;
    struct timeval start;
    struct timeval end;
} Timer;

/**
 * Diff time as epoch
 * @param start
 * @param end
 * @return
 */
long timer_diff_time(struct timeval start, struct timeval end);

/**
 * Elapsed time as epoch
 * @param t
 * @return
 */
long timer_get_delta(const Timer *t);

void timer_from_epoch(long epoch, Timer *result);

int timer_ms_to_ss(int ms);
int timer_ms_to_s(int ms);
int timer_ms_to_m(int ms);
int timer_ms_to_h(int ms);

Timer * new_timer();
void timer_start(Timer *t);
void timer_update(Timer *t);
void timer_sprintf(const Timer *t, char *buffer);
void timer_fprint(const Timer *t, FILE *stream);
void timer_print(const Timer *t);

#endif /* STEALTH_TIMER_H */
