/*
 *   This file is part of Stealth game
 *    Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include "model/Score.h"
#include "core/Timer.h"

size_t score_read(const char *path, Score *scores, int n){
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        /*perror("fopen");*/
        return 0;
    }
    size_t ret = fread(scores, sizeof(Score), n, fp);
    fclose(fp);
    return ret;
}

size_t score_write(const char *path, Score *scores, int n){
    FILE *fp = fopen(path, "wb");
    if (!fp) {
        /*perror("fopen");*/
        return 0;
    }
    size_t ret = fwrite(scores, sizeof(Score), n, fp);
    fclose(fp);
    return ret;
}

int score_cmp_mana(const void *s1, const void *s2){
    return ((const Score *) s2)->mana - ((const Score*) s1)->mana;
}

int score_cmp_time(const void *s1, const void *s2){
    long test =  ((const Score *) s1)->time - ((const Score*) s2)->time;
    return test == 0 ? 0 : test < 0 ? -1 : 1;
}

int score_get_mana(const Score *score){
    return score->mana;
}
long score_get_time(const Score *score){
    return score->time;
}

void score_sprintf_mana(const Score *score, char *buff){
    sprintf(buff, "%20d", score_get_mana(score));
}

void score_sprintf_time(const Score *score, char *buff){
    Timer timer;
    timer_from_epoch(score_get_time(score), &timer);
    timer_sprintf(&timer, buff);
}