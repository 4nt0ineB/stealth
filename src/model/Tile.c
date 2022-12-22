#include <assert.h>
#include "model/Tile.h"

int tile_get_type(const Tile *tile){
    assert(tile);
    return tile->type;
}

void tile_set_type(Tile *tile, TileType type){
    assert(tile);
    tile->type = type;
}
