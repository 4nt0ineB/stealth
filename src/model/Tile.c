#include <assert.h>
#include "model/Tile.h"


void print_tile(Tile tile){
    switch (tile.type) {
        case MANA:
            printf("M");
            break;

        case WALL:
            printf("X");
            break;

        case RELIC:
            printf("R");
            break;
        case EMPTY:
            printf(" ");
    }
}

int tile_get_type(const Tile *tile){
    assert(tile);
    return tile->type;
}

void tile_set_type(Tile *tile, TileType type){
    assert(tile);
    tile->type = type;
}
