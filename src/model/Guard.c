#include "core/Settings.h"

#include "model/Guard.h"


#include <stdlib.h>

void init_guard(Guard *new_guard, int x, int y) {
    new_guard->x = x;
    new_guard->y = y;
    new_guard->speed = SPS / ((rand() % 6) + 3);
    new_guard->direction = new_dir();
    new_guard->panick_mode = 0;
}

void change_direction(Guard *guard) {
    Direction dir;
    while ((dir = new_dir()) == guard->direction);
    guard->direction = dir;
}

void change_speed(Guard *guard) {
    guard->speed = SPS / ((rand() % 6) + 3);
}

void move(Guard *guard) {
    /*switch (guard->direction){
        case LEFT:
            guard->x -= guard->speed;
            break;
        case RIGHT:
            guard->x += guard->speed;
            break;
        case DOWN:
            guard->y += guard->speed;
            break;
        case UP:
            guard->y -= guard->speed;
            break;
    }*/
}

void change_guard(Guard *guard) {
    change_direction(guard);
    change_speed(guard);
}

void make_panick(Guard *guard) {
    guard->panick_mode = 1;
}

void unpanick(Guard *guard) {
    guard->panick_mode = 0;
}


void normal_mode(Guard *guard) {
    if (rand() % 50 == 1)
        change_guard(guard);
    /* TODO DETECTION FUNCTION */
    move(guard);
}

void panick_mode(Guard *guard) {
    if (rand() % 50 == 1)
        change_guard(guard);
    guard->speed = SPS;
    /* TODO DETECTION FUNCTION */
    move(guard);
}

void guard_move(Guard *guard) {
    if (guard->panick_mode)
        panick_mode(guard);
    else if (!panick_mode && guard->speed == SPS) {
        /* Just got out of panick mode so must reset to a normal
        mode speed before calling normal_mode function */
        change_speed(guard);
        normal_mode(guard);
    } else
        normal_mode(guard);
}

