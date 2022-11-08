#include "view/Menu.h"
#include "model/Room.h"
#include "Test.h"
#include "model/Character.h"
#include "model/Guard.h"
#include "model/Relic.h"
#include "model/Direction.h"
#include "model/Tile.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "controller/Controller.h"

int main(int args, char *argv[]){
    srand(time(NULL));
    /* TESTING ROOM FUNCTIONS */
    Room room;
    init_room(&room);
    print_room(room);
    return slealth_controller(&room);
    return 0;
}