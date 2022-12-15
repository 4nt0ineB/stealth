#include "model/Relic.h"

void init_relic(Relic *relic, Position pos){
    assert(relic);
    relic->stolen = 0;
    relic->noticed = 0;
    relic->position = pos;
}

void relic_steal(Relic *relic){
    assert(relic);
    relic->stolen = 1;
}

int relic_is_stolen(const Relic *relic){
    assert(relic);
    return relic->stolen;
}