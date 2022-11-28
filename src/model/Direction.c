#include "model/Direction.h"
#include "core/Util.h"

const double direction_factor[9][2] = {
        {0, 0},
        {0, -1},
        {SQRT2 / 2, -(SQRT2 / 2)},
        {1, 0},
        { SQRT2 / 2, SQRT2 / 2},
        {0, 1},
        { -(SQRT2 / 2), SQRT2 / 2},
        {-1, 0},
        {-(SQRT2 / 2), -(SQRT2 / 2)},
};

Direction direction_previous(Direction direction){
    if(direction == STILL) return STILL;
    return modulo((int) direction - 2, 8) + 1;
}

Direction direction_next(Direction direction){
    if(direction == STILL) return STILL;
    return modulo((int) direction + 2, 8) - 1;
}