#include "model/Tile.h"

void init_tile(Tile *new_tile, TileType type){
    new_tile->type = type;
    switch (new_tile->type){
        case WALL:
            new_tile->caught = 0;
            new_tile->mana = 0;
            break;
        case TILE:
            new_tile->caught = 0;
            new_tile->mana = 10; /* Randomize it later */
            break;
        case RELIC:
            new_tile->caught = 0;
            new_tile->mana = 0;
            break;
    }
}

void print_tile(Tile tile){
    switch (tile.type) {
        case TILE:
            printf(" ");
            break;
        
        case WALL:
            printf("X");
            break;
        
        case RELIC:
            printf("R");
            break;
    }
}


