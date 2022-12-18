/*
 *   This file is part of Stealth game
 *    Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include "model/Score.h"

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
    return 0;
}

int score_cmp_mana(const void *s1, const void *s2){
    return ((const Score *) s2)->mana - ((const Score*) s1)->mana;
}

int score_cmp_time(const void *s1, const void *s2){
    return (int) ((const Score *) s2)->time - (int) ((const Score*) s1)->time;
}

int score_get_mana(const Score *score){
    return score->mana;
}
int score_get_time(const Score *score){
    return score->time;
}