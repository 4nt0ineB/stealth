
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
        guard_init(&(new_room->guards[i])
                   , int_rand(1, ROOM_WIDTH - 1) /* @Todo test if position is not a wall before */
                   , int_rand(1, ROOM_WIDTH - 1));
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


int room_resolve_collision(Room *room, Position *position){
    assert(room && position);
    Position tl = {
            .x = MAX(0, (int) position->x - 1),
            .y = MAX(0, (int) position->y - 1)
    };
    Position br = {
            .x = MIN(ROOM_WIDTH, (int) position->x + 1),
            .y = MIN(ROOM_HEIGHT, (int) position->y + 1)
    };
    int collide = 0;
    int y, x;
    for(y = (int) tl.y; y <= br.y; y++){
        for(x = (int) tl.x; x <= br.x; x++){
            if(room->tiles[y][x].type == WALL){
                /* nearest point of the square */
                Position nearest = {
                        .x = CLAMP(x, position->x, x + 1),
                        .y = CLAMP(y, position->y, y + 1)
                };
                /* distance of this point to the center of the circle */
                Position distance;
                position_sub(position, &nearest,  &distance);
                double vector_norm = vector_mag(&distance);
                if(0.5 - vector_norm > 0){
                    collide = 1;
                    position->x = nearest.x + 0.5 * (distance.x / vector_norm);
                    position->y = nearest.y + 0.5 * (distance.y / vector_norm);
                }
            }
        }
    }
    return collide;
}

static void entity_move(Position *position, double speed, Direction direction){
    assert(position);
    position->x += COMPUTE_MOVE_DIST(speed) * direction_factor[direction][0];
    position->y += COMPUTE_MOVE_DIST(speed) * direction_factor[direction][1];
}

void room_move_player(Room *room, Direction direction){
    assert(room);
    character_update_speed(&room->player, direction);
    entity_move(&room->player.position, room->player.speed, direction);
    if(room_resolve_collision(room, &room->player.position)){  /* collided */
        /* The player is hitting the wall, so the speed must be reset to init speed */
        character_update_speed(&room->player, STILL);
    }
}

void room_move_guards(Room *room){
    int i;
    for(i = 0; i < GUARD_NUMBER; i++){
        guard_update_speed(&room->guards[i]);
        entity_move(&room->guards[i].position
                    , room->guards[i].speed
                    , guard_update_direction(&room->guards[i]));
        if(room_resolve_collision(room, &room->guards[i].position)){ /* collided */
            /* The guard is hitting the wall, so the speed and the direction must be updated */
            guard_update_speed(&room->guards[i]);
            guard_update_direction(&room->guards[i]);
        }
    }
}