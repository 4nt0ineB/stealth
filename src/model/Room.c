
#include <assert.h>
#include <math.h>

#include "model/Room.h"
#include "core/Util.h"

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
            init_tile(&(room->tiles[i][x_min + compartiment_x]), WALL);
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
            init_tile(&(room->tiles[compartiment_y + y_min][i]), WALL);
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
                init_tile(&(new_room->tiles[i][j]), WALL);
            else
                init_tile(&(new_room->tiles[i][j]), TILE);
        }
    }

    /* Generate inside walls*/
    generate_room(new_room, 1, 1, ROOM_WIDTH - 2, ROOM_HEIGHT - 2);

    character_init(&(new_room->player), 2,  2); /* Not good values just to test*/
    for(i = 0; i < GUARD_NUMBER; i++)
        init_guard(&(new_room->guards[i]), 0, 0); /* Same about values*/
}

void print_room(Room room){
    int i, j;
    for (i = 0; i < ROOM_HEIGHT; i++){
        for (j = 0; j < ROOM_WIDTH; j++) {
            if (i == room.player.position.y && j == room.player.position.x)
                printf("P");
            else print_tile(room.tiles[i][j]);
        }
        printf("\n");
    }
    printf("Room Height : %d  Width : %d\n", ROOM_HEIGHT, ROOM_WIDTH);
}

/**
 * Get the sq
 * @param position
 * @param result_indexes
 */
static void get_tile_index_from_coordinates(Position *position, Position *result_indexes){
    assert(position && result_indexes);
    result_indexes->x = (int) position->x;
    result_indexes->y = (int) position->y;
}

void room_move_player(Room *room, Direction direction){
    assert(room);
    Position player_tile;
    /* Get the current cell index the player is on */
    get_tile_index_from_coordinates(&room->player.position, &player_tile);
    character_update_speed(&room->player, direction);
    /* Collision detection */
    Position vector;
    vector.x = COMPUTE_MOVE_DIST(room->player.speed) * direction_factor[direction][0];
    vector.y = COMPUTE_MOVE_DIST(room->player.speed) * direction_factor[direction][1];
    /* Test collisions */
    /* To put in another method to handle any entity */
    /* S */
    Position new_pos, max_pos;

    new_pos.x = room->player.position.x + vector.x;
    new_pos.y = room->player.position.y + vector.y;

    Position target_tile = new_pos;
    int y, x;
    for(y = (int) target_tile.y - 1; y <= target_tile.y + 1; y++){
        for(x = (int) target_tile.x - 1; x <= target_tile.x + 1; x++){
            if(room->tiles[y][x].type == WALL){
                Position nearest = {
                        .x = MAX(x, MIN((double)new_pos.x, (double) (x + 1))),
                        .y = MAX(y, MIN((double)new_pos.y, (double) (y + 1)))
                };
                Position ray;
                position_sub(&nearest, &new_pos, &ray);
                double overlap = 0.5 - vector_mag(&ray);

                if(overlap > 0){
                    printf("Overlap : %f\n", overlap);
                    Position offset = {
                        .x = nearest.x - new_pos.x,
                        .y = nearest.y - new_pos.y
                    };
                    double distance = sqrt(pow(offset.x, 2) + pow(offset.y, 2));
                    Position direction = {
                            .x = offset.x / distance,
                            .y = offset.y / distance
                    };
                    double len = 0.5 - distance;
                    new_pos.x = room->player.position.x + len * direction.x;
                    new_pos.y = room->player.position.y + len * direction.y;
                }
            }
        }

    }

    room->player.position.x =  new_pos.x;
    room->player.position.y =  new_pos.y;
/*
    room->player.position.x += vector.x;
    room->player.position.y += vector.y;*/



}