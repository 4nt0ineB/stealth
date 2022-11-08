/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include "core/Util.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int int_rand(int a, int b){
    return (rand() % (b + 1 - a)) + a;
}

float float_rand(float min, float max){
    return min + (rand() / (float) RAND_MAX) * (max - min);
}

int modulo(int a, int b) {
    int mod;
    assert(b != 0);
    mod = a % b;
    return mod >= 0 ? mod : mod + b;
}

int greatest_common_multiplier_less_than(int a, int b, int max){
    int lcm, found, lastvalid;
    lcm = lastvalid = MIN(a, b);
    found = 0;
    while(!found && lcm <= max){
        found = 1;
        if(lcm % a != 0 || lcm % b != 0)
            found = 0;
        if(found) {
            lastvalid = lcm;
            found = 0;
        }
        lcm++;
    }
    return lastvalid;
}