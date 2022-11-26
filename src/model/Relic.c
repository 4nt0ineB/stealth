#include "model/Relic.h"

void init_relic(Relic *relic, Position pos){
    assert(relic);
    relic->taken = 0;
    relic->position = pos;
}
