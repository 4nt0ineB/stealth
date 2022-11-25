
#include "model/Room.h"
#include "controller/Controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int args, char *argv[]){
    srand(time(NULL));
    Room room;
    room_init(&room);
    return slealth_controller(&room);
}