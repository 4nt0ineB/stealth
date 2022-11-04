#include "Room.h"


Room init_room(){
    Room new_room;
    int i, j;

    for (i = 0; i < ROOM_HEIGHT; i++){
        for (j = 0; j < ROOM_WIDTH; j++){
            if (i == 0 || j == 0 || i == ROOM_HEIGHT - 1 || j == ROOM_WIDTH - 1)
                new_room.room[i][j] = init_tile(WALL);
        }
    }
    /* Puting Walls all over the rectangle sides */
    /* TODO GENERATION FUNCTION GIVEN IN PROJECT PDF 
    (I didn't understand it so didn't do it right now)*/
}
