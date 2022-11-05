#include "Menu.h"
#include "Room.h"
#include "Test.h"
#include "Character.h"
#include "Guard.h"
#include "Relic.h"
#include "Direction.h"
#include "Tile.h"
#include "graphics/Graphic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int args, char *argv[]){
    srand(time(NULL));
    /* TESTING ROOM FUNCTIONS */
    Room room;
    init_room(&room);
    print_room(room);
    return 0;
}