#include "Tile.h"

Tile init_tile(TileType tile_type){
    Tile new_tile;
    new_tile.tile_type = tile_type;
    switch (new_tile.tile_type){
        case WALL:
            new_tile.caught = 0;
            new_tile.mana = 0;
            break;
        case TILE:
            new_tile.caught = 0;
            new_tile.mana = 10; /* Randomize it later */
            break;
        case RELIC:
            new_tile.caught = 0;
            new_tile.mana = 0;
            break;
    }
    return new_tile;
}

