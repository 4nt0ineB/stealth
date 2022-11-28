#include "model/Relic.h"

void init_relic(Relic *relic, Position pos){
    assert(relic);
    relic->taken = 0;
    relic->noticed = 0;
    relic->position = pos;
}

void take_relic(Relic *relic){
    assert(relic);
    relic->taken = 1;
}