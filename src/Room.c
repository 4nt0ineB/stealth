#include "Room.h"


void generate_room(Room *room, int x_min, int y_min, int x_max, int y_max){
    int i, j;
    int x = x_max - x_min;
    int y = y_max - y_min;
    int compartiment_x = x / 2;
    int compartiment_y = y / 2;
    if (x > y){
        /* Compartiment from x distance */

        if (x < 2 * MINSIDE + 1)
            return;
        else if (x < 4 * MINSIDE){
            if (rand() % 2)
                return;
        }
        
        if (rand() % 2){
            /* Extremity of y_min choosed*/
            i = y_min + 3;
            j = y_max;
        }
        else {
            /* Extremity of y_max choosed */
            i = y_min;
            j = y_max - 3;
        }
        /* Creating the Wall that divide the 2 spaces at x / 2*/
        for (; i < j; i++) 
            init_tile(&(room->room[i][x_min + compartiment_x]), WALL);
        generate_room(room, x_min, y_min, compartiment_x + x_min, y_max);
        generate_room(room, compartiment_x + x_min, y_min, x_max, y_max);
    } 
    else {
        /* Compartiment from y distance */
        if (y < 2 * MINSIDE + 1)
            return;
        else if (y < 4 * MINSIDE){
            if (rand() % 2)
                return;
        }
        
        if (rand() % 2){
            /* Extremity of y_min choosed*/
            i = x_min + 3;
            j = x_max;
        }
        else {
            /* Extremity of y_max choosed */
            i = x_min;
            j = x_max - 3;
        }
        /* Creating the Wall that divide the 2 spaces at x / 2*/
        for (; i < j; i++) 
            init_tile(&(room->room[compartiment_y + y_min][i]), WALL);
        generate_room(room, x_min, y_min, x_min, y_min + compartiment_y);
        generate_room(room, x_min, y_min + compartiment_y, x_max, y_max);
    }   
}

void init_room(Room *new_room){
    int i, j;

    /* Puting outside Walls all over the rectangle sides */
    for (i = 0; i < ROOM_HEIGHT; i++){
        for (j = 0; j < ROOM_WIDTH; j++){
            if (i == 0 || j == 0 || j == ROOM_WIDTH - 1 || i == ROOM_HEIGHT - 1)
                init_tile(&(new_room->room[i][j]), WALL);
            else 
                init_tile(&(new_room->room[i][j]), TILE);
        }
    }
    
    /* Generate inside walls*/
    generate_room(new_room, 1, 1, ROOM_WIDTH - 2, ROOM_HEIGHT - 2);

    init_character(&(new_room->player), 2, 2); /* Not good values just to test*/
    for(i = 0; i < GUARD_NUMBER; i++)
        init_guard(&(new_room->guards[i]), 0, 0); /* Same about values*/
}

void print_room(Room room){
    int i, j;
    for (i = 0; i < ROOM_HEIGHT; i++){
        for (j = 0; j < ROOM_WIDTH; j++) {
            if (i == room.player.y && j == room.player.x)
                printf("P");
            else print_tile(room.room[i][j]);
        }
        printf("\n");
    } 
    printf("Room Height : %d  Width : %d\n", ROOM_HEIGHT, ROOM_WIDTH);
}
