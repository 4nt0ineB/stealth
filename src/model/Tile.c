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


