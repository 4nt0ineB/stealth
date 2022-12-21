/*
 *   This file is part of Stealth game
 *    Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include "core/Timer.h"
#include <stdlib.h>
/*
 *
 * Note pour justification
 * d'utilisation de gettimeofday plutôt que clock_gettime():
 * d'après le man avec l'utilisation de CLOCK_REALTIME
 * la capture du temps est identique à gettimeofday.
 *
 *
 *
 */


long timer_diff_time(struct timeval start, struct timeval end){
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
}

long timer_get_delta(const Timer *t){
    return timer_diff_time(t->start, t->end);
}

void timer_from_epoch(long epoch, Timer *result){
    result->start.tv_usec = 0;
    result->start.tv_sec = 0;
    result->end.tv_sec = epoch / 1000;
    result->end.tv_usec = (epoch - result->end.tv_sec * 1000) * 1000;
}

int timer_ms_to_ss(int ms) {
    return modulo(ms / 10, 100);
}

int timer_ms_to_s(int ms) {
    return modulo(ms / 1000, 60);
}

int timer_ms_to_m(int ms) {
    return modulo(ms / (1000 * 60), 60);
}

int timer_ms_to_h(int ms) {
    return modulo(ms / (3.6 * 1000000), 100);
}

Timer * new_timer(){
    return (Timer *) calloc(1, sizeof(Timer));
}

void timer_start(Timer *t){
    if(!t) return;
    gettimeofday(&t->start, NULL);
}

void timer_update(Timer *t){
    if(!t) return;
    gettimeofday(&t->end, NULL);
}


void timer_sprintf(const Timer *t, char *buffer){
    int s, m, ss;
    unsigned long ms = timer_get_delta(t);
    ss = timer_ms_to_ss((int) ms);
    s = timer_ms_to_s(ms);
    m = timer_ms_to_m(ms);
    /* @Todo improve function by not printing zero values */
    sprintf(buffer, "%02d:%02d:%02d", m, s, ss);
}

void timer_fprint(const Timer *t, FILE * stream){
    int ss, s, m, h;
    unsigned long ms;
    ms = timer_get_delta(t);
    ss = timer_ms_to_ss((int) ms);
    s = timer_ms_to_s((int) ms);
    m = timer_ms_to_m((int) ms);
    h = timer_ms_to_h((int) ms);
    fprintf(stream, "%02d:%02d:%02d:%02d", h, m, s, ss);
}

void timer_print(const Timer *t){
    timer_fprint(t, stdout);
}