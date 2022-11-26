
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
        /* Creating the Wall that divide the 2 spaces at x/2 */
        for (; i < j; i++)
            room->tiles[i][x_min + compartiment_x].type = WALL;
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
        /* Creating the Wall that divide the 2 spaces at x/2 */
        for (; i < j; i++)
            room->tiles[compartiment_y + y_min][i].type = WALL;
        generate_room(room, x_min, y_min, x_min, y_min + compartiment_y);
        generate_room(room, x_min, y_min + compartiment_y, x_max, y_max);
    }
}

/**
 * Get a random tile index of a given type
 * @param room
 * @param tile_type the type of tile to get
 * @return 1 if indexes found, otherwise 0
 */
static int room_random_position(Room *room, TileType tile_type, int *res_x, int *res_y){
    assert(room);
    do{
        *res_x = int_rand(1, ROOM_WIDTH - 1);
        *res_y = int_rand(1, ROOM_HEIGHT - 1);
    }while(room->tiles[*res_y][*res_x].type != tile_type);
    return 1;
}

void room_init(Room *room){
    int i, j, x, y;

    /* Puting outside Walls all over the rectangle sides */
    for (i = 0; i < ROOM_HEIGHT; i++){
        for (j = 0; j < ROOM_WIDTH; j++){
            if (i == 0 || j == 0 || j == ROOM_WIDTH - 1 || i == ROOM_HEIGHT - 1)
                room->tiles[i][j].type = WALL;
            else
                room->tiles[i][j].type = EMPTY;
        }
    }

    /* Generate inside walls*/
    generate_room(room, 1, 1, ROOM_WIDTH - 2, ROOM_HEIGHT - 2);

    /* player */
    character_init(&(room->player), 2, 2);
    /* guards */
    for(i = 0; i < GUARD_NUMBER; i++){
        room_random_position(room, EMPTY, &x, &y);
        guard_init(&(room->guards[i]) , x, y);
    }
    /* mana */
    for(i = 0; i < MANA_TILES_NUMBER; i++){
        room_random_position(room, EMPTY, &x, &y);
        room->tiles[y][x].type = MANA;
    }
}

void room_print(Room room){
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
                /* Distance to this point */
                double norm = vector_mag(&distance);
                if(0.5 - norm > 0){
                    collide = 1;
                    /* Set back the position of the circle to the legit position */
                    position->x = nearest.x + 0.5 * (distance.x / norm);
                    position->y = nearest.y + 0.5 * (distance.y / norm);
                }
            }
        }
    }
    return collide;
}

/**
 * Moves the entity by a given Velocity (speed and direction)
 * @param position
 * @param speed
 * @param direction
 */
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
    assert(room);
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

int room_tile_between(const Room *room, const Position *p1, const Position *p2, TileType tile_type){

    int i;
    double xa;
    Position pos_a, tmp;

    for(i = 0; i < ROOM_HEIGHT; i++){
        /* check if on segment */
        xa = (i - p1->y) / (p2->y - p1->y);
        if(xa >= 0 && xa <= 1){
            pos_a.y = i;
            position_interpolate_with_x(p1, p2, &pos_a);
            tmp.x = (int) pos_a.x;
            tmp.y = (int) pos_a.y;
            if(room->tiles[(int) tmp.y][(int) tmp.x].type == tile_type)
                return 1;
        }
    }

    for(i = 0; i < ROOM_WIDTH; i++){
        xa = (i - p1->x) / (p2->x - p1->x);
        /* check if on segment */
        if(xa >= 0 && xa <= 1){
            pos_a.x = i;
            position_interpolate_with_y(p1, p2, &pos_a);
            tmp.x = (int) pos_a.x;
            tmp.y = (int) pos_a.y;
            if(room->tiles[(int) tmp.y][(int) tmp.x].type == tile_type)
                return 1;
        }
    }

    return 0;
}

void room_check_guard_panic(Room *room){
    int i;
    for(i = 0; i < GUARD_NUMBER; i++){
        if(position_dist(&room->guards[i].position, &room->player.position)
           < guard_view_range(&room->guards[i])
        && !room_tile_between(room, &room->guards[i].position, &room->player.position, WALL)){
            guard_panick(&room->guards[i]);
        }else{
            guard_unpanick(&room->guards[i]);
        }
    }
}

void room_check_player(Room *room){
    Position current_tile = {
            .x = (int) room->player.position.x,
            .y = (int) room->player.position.y
    };
    Tile *tile = &room->tiles[(int) current_tile.y][(int) current_tile.x];
    if(tile->type == MANA){
        room->player.mana += 1;
        tile->type = EMPTY;
    }
}