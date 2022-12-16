
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
        for (; i <= j; i++)
            room->tiles[i][x_min + compartiment_x].type = WALL;
        generate_room(room, x_min, y_min, compartiment_x + x_min - 1, y_max);
        generate_room(room, compartiment_x + x_min + 1, y_min, x_max, y_max);
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
        for (; i <= j; i++)
            room->tiles[compartiment_y + y_min][i].type = WALL;
        generate_room(room, x_min, y_min, x_min, y_min + compartiment_y - 1);
        generate_room(room, x_min, y_min + compartiment_y + 1, x_max, y_max);
    }
}


int room_get_tile_type(const Room *room, int i, int j){
    assert(i < ROOM_HEIGHT && i >= 0);
    assert(j < ROOM_WIDTH && j >= 0);
    return tile_get_type(&room->tiles[i][j]);
}

int room_random_position(const Room *room, TileType tile_type, int *res_x, int *res_y){
    assert(room);
    do{
        *res_x = int_rand(1, ROOM_WIDTH - 1);
        *res_y = int_rand(1, ROOM_HEIGHT - 1);
    }while(room_get_tile_type(room, *res_y, *res_x) != tile_type);
    return 1;
}

void room_init(Room *room){
    int i, j;
    /* Putting outview->side Walls all over the rectangle view->sides */
    for (i = 0; i < ROOM_HEIGHT; i++){
        for (j = 0; j < ROOM_WIDTH; j++){
            if (i == 0 || j == 0 || j == ROOM_WIDTH - 1 || i == ROOM_HEIGHT - 1)
                tile_set_type(&room->tiles[i][j], WALL);
            else
                tile_set_type(&room->tiles[i][j], EMPTY);
        }
    }
    /* Generate inview->side walls*/
    generate_room(room, 1, 1, ROOM_WIDTH - 1, ROOM_HEIGHT - 1);
}

int room_resolve_collision(Room *room, Position *position){
    assert(room && position);
    /* top left tile */
    Position tl = {
            .x = MAX(0, (int) position->x - 1),
            .y = MAX(0, (int) position->y - 1)
    };
    /* bottom right tile */
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

Tile * room_get_tile(Room *room, int i, int j){
    assert(i < ROOM_HEIGHT && i >= 0);
    assert(j < ROOM_WIDTH && j >= 0);
    return &room->tiles[i][j];
}

void room_add_mana(Room *room, int amount){
    int y, x;
    int i;
    for(i = 0; i < amount; i++){
        room_random_position(room, EMPTY, &x, &y);
        room->tiles[y][x].type = MANA;
    }
}