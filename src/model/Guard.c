#include "core/Settings.h"

#include "model/Guard.h"
#include "core/Util.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void guard_init(Guard *guard, int x, int y) {
    guard->position.x = x;
    guard->position.y = y;
    guard->speed = float_rand(GUARD_MIN_SPEED, GUARD_MAX_SPEED);
    guard->direction = STILL;
    guard->panick_mode = 0;
}

void guard_update_speed(Guard *guard){
    assert(guard);
    double speed = guard->speed;
    if(int_rand(0, 99) <= GUARD_NEW_DIR_PROBABILITY * 100){
        speed = float_rand(GUARD_MIN_SPEED, GUARD_MAX_SPEED);
    }
    /*if(guard->panick_mode){
        speed = ...
    }*/
    guard->speed = speed;
}

Direction guard_update_direction(Guard *guard){
    assert(guard);
    if(int_rand(0, 99) <= GUARD_NEW_DIR_PROBABILITY * 100){
        return guard->direction = int_rand(STILL, NO);
    }
    return guard->direction;
}

void guard_panick(Guard *guard){
    assert(guard);
    guard->panick_mode = 1;
}

void guard_unpanick(Guard *guard){
    assert(guard);
    guard->panick_mode = 1;
}


