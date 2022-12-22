#include "model/Guard.h"

void guard_init(Guard *guard, int x, int y) {
    position_init(&guard->position, x, y);
    guard->speed = float_rand(GUARD_MIN_SPEED, GUARD_MAX_SPEED);
    guard->direction = STILL;
    guard_unpanic(guard);
    guard_reset_panic_count(guard);
}

void guard_update_speed(Guard *guard){
    assert(guard);
    double speed = guard->speed;
    if(int_rand(0, 99) <= GUARD_NEW_DIR_PROBABILITY * 100){
        speed = float_rand(GUARD_MIN_SPEED, GUARD_MAX_SPEED);
    }
    if(guard->panic_mode){
        speed = GUARD_PANIC_SPEED;
    }
    guard->speed = speed;
}

Direction guard_update_direction(Guard *guard){
    assert(guard);
    if(int_rand(0, 99) <= GUARD_NEW_DIR_PROBABILITY * 100){
        return guard->direction = int_rand(STILL, NO);
    }
    return guard->direction;
}

double guard_view_range(const Guard *guard){
    if(guard->panic_mode)
        return GUARD_PANIC_VIEW_RANGE;
    return GUARD_VIEW_RANGE;
}

void guard_panic(Guard *guard){
    assert(guard);
    guard->panic_mode = 1;
}

void guard_unpanic(Guard *guard){
    assert(guard);
    guard->panic_mode = 0;
}

void guard_reset_panic_count(Guard *guard){
    assert(guard);
    guard->panic_count = 0;
}

void guard_update_panic_count(Guard *guard){
    assert(guard);
    if (guard->panic_mode){
        if (guard->panic_count >= GUARD_PANIC_DURATION){
            guard_reset_panic_count(guard);
            guard_unpanic(guard);
        }
        guard->panic_count += 1;
    }
}


int guard_is_panicking(const Guard *guard){
    return guard->panic_mode;
}
