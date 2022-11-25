/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include "view/View.h"
#include "core/Settings.h"
#include "core/Util.h"
#include "core/Timer.h"
#include <assert.h>

/**
 * There is only one possible running window with MLV so why not use a global
 */
static struct {
    /**
     * @brief Zone d'information définit manuellement sous forme
     *  de pourcentage comme en css. Ex x, y et 15 (80% de la largeur) 25 (25% de la hauteur);
     *  (centré à par rapport à window )
     *
     */
    Rectangle info_area;
    /**
     * @brief zone restante à calculer dynamiquement
     * après définition de la zone d'information
     *
     */
    Rectangle available_area;
    /**
     * @brief zone de jeu à calculer dynamiquement d'après la zone
     * disponible et en fonction des constantes du nombre de tuiles par côté.
     * (centré à par rapport à available_area )
     */
    Rectangle game_area;
    MLV_Color bg_color;
    MLV_Font *font;
    Timer *timer;
    /*
        ..later on
    */
} View;

void view_init(){
    View.bg_color = MLV_COLOR_BLACK;
    MLV_create_window("Slealth", "", 1, 1);

    /* Default frame rate */
    MLV_change_frame_rate( FPS );
    /* Set window dimension to default values */
    view_update_size((MLV_get_desktop_width() * DEFAULT_WIN_W_PERCENT) / 100,
                     (MLV_get_desktop_height() * DEFAULT_WIN_H_PERCENT) / 100);
    MLV_draw_filled_rectangle(
            0
            , 0
            , MLV_get_window_width()
            , MLV_get_window_height()
            , View.bg_color);
    View.timer = new_timer();
    timer_start(View.timer);
}


void view_draw_info(const Room *room){
    static char buffer[40] = {0};
    draw_rectangle(&View.info_area, MLV_COLOR_GRAY50);
    timer_sprintf(View.timer, buffer);
    MLV_draw_text_with_font(
            View.info_area.w / 2
            , View.info_area.origin.y + View.info_area.h / 3
            , buffer
            , View.font
            ,MLV_COLOR_BLACK);
}

void view_update_time(){
    timer_update(View.timer);
}

void view_update_size(int w, int h){
    assert(w > 0 && h > 0);
    /* Update font */
    if(View.font)
        MLV_free_font(View.font);
    View.font = MLV_load_font(
            RESOURCE("fonts/militech.ttf")
            , (w * FONT_HEIGHT_PERCENT) / 100);
    /* Info area */
    View.info_area.w = (w * DEFAULT_INFO_W_PERCENT) / 100;
    View.info_area.h = (h * DEFAULT_INFO_H_PERCENT) / 100;
    /* Available area, for the calculation of the game area dimensions  */
    View.available_area.origin.y = View.info_area.h;
    View.available_area.w = w;
    View.available_area.h = h - View.info_area.h;
    /* Compute SIDE from the new available area dimensions  */
    SIDE = greatest_common_multiplier_less_than(
            ROOM_WIDTH,
            ROOM_HEIGHT,
            MIN(View.available_area.w, View.available_area.h));
    SIDE = SIDE / MIN(ROOM_WIDTH, ROOM_HEIGHT);
    /* Game area */
    View.game_area.w = SIDE * ROOM_WIDTH;
    View.game_area.h = SIDE * ROOM_HEIGHT;
    /* Center the game area in the available area */
    View.game_area.origin.x = View.available_area.origin.x + (View.available_area.w - View.game_area.w) / 2;
    View.game_area.origin.y = View.available_area.origin.y + (View.available_area.h - View.game_area.h) / 2;
    MLV_change_window_size(w, h);
}

void view_draw_util(){
    static char buffer[10] = {0};
    /* */
    draw_rectangle(&View.available_area, MLV_rgba(41, 52, 71,255));
    sprintf(buffer, "%d fps", MLV_get_frame_rate());
    MLV_draw_text_with_font(View.info_area.w - View.info_area.w / 5
                            , View.info_area.origin.y + View.info_area.h / 3
                            , buffer
                            , View.font
                            ,MLV_COLOR_BLACK);
}

static int view_get_absolute_position(const Position *position, Position *result){
    result->x = View.game_area.origin.x + position->x * SIDE;
    result->y = View.game_area.origin.y + position->y * SIDE;
}

void view_draw_player(const Character *character){
    Position pos;
    view_get_absolute_position(&character->position, &pos);
    MLV_draw_filled_circle(pos.x, pos.y, SIDE / 2, MLV_COLOR_RED);
}

void view_draw_guard(const Guard *guard){
    Position pos;
    MLV_Color color;
    color = MLV_COLOR_BLUE;
    if(guard->panic_mode) color = MLV_COLOR_YELLOW;
    view_get_absolute_position(&guard->position, &pos);
    MLV_draw_filled_circle(
            pos.x
            , pos.y
            , SIDE / 2
            , color);

    MLV_draw_circle(
            pos.x
            , pos.y
            , SIDE * 4 /* @Todo: to change. The guard must have a vision range */
            , color);
}

void view_draw_room(const Room *room){
    /*
     * The absolute position of any object of the room to be drawn
     * is determined here !
     */
    int i, j;
    /* The Background */
    draw_rectangle(&View.game_area, MLV_COLOR_DARK_GREY);

    /* The Grid */
    /* Draw lines */
    int ox = View.game_area.origin.x;
    int oy = View.game_area.origin.y;
    int thickness = 1;
    for(i = 0; i < ROOM_HEIGHT; i++){
        MLV_draw_filled_rectangle(
                ox
                , oy + i * SIDE
                , View.game_area.w
                , thickness
                , MLV_COLOR_BLACK);
    }
    /* Draw columns */
    for(j = 0; j < ROOM_WIDTH; j++){
        MLV_draw_filled_rectangle(
                ox + j * SIDE
                , oy
                , thickness
                , View.game_area.h
                , MLV_COLOR_BLACK);
    }

    for(i = 0; i < ROOM_HEIGHT; i++){
        for(j = 0; j < ROOM_WIDTH; j++){
            /*
             * @Todo delegate
             * */
            switch (room->tiles[i][j].type) {
                case WALL:
                    MLV_draw_filled_rectangle(
                            ox + j * SIDE
                            , oy + i * SIDE
                            , SIDE
                            , SIDE
                            , MLV_COLOR_BLACK
                            );
                    break;
                case TILE:
                case RELIC:
                    break;
            }
        }
    }

    /* Player */
    view_draw_player(&room->player);

    /* Guards */
    for(i = 0; i < GUARD_NUMBER; i++){
        view_draw_guard(&room->guards[i]);

    }
}

void view_free(){
    MLV_free_font(View.font);
    MLV_free_window();
}

void draw_rectangle(const Rectangle *rectangle, const MLV_Color color){
    MLV_draw_filled_rectangle(
            rectangle->origin.x, rectangle->origin.y
            , rectangle->w, rectangle->h, color);
}


void draw_intersections_with_tiles(const Room *room, const Position *p1, const Position *p2){
    assert(p1 && p2);
    /* draw line between positions */
    Position abs_pos1, abs_pos2;
    view_get_absolute_position(p1, &abs_pos1);
    view_get_absolute_position(p2, &abs_pos2);
    MLV_draw_line(abs_pos1.x,abs_pos1.y,
                  abs_pos2.x,abs_pos2.y,MLV_COLOR_BLACK);

    /* draw intersection */
    int i;
    double xa;
    Position abs_p0, abs_p1, pos_a;
    Position tmp;

    for(i = 0; i < ROOM_HEIGHT; i++){
        xa = (i - p1->y) / (p2->y - p1->y);
        if(xa >= 0 && xa <= 1){ /* check if on segment */
            pos_a.y = i;
            position_interpolate_with_x(p1, p2, &pos_a);
            tmp.x = (int) pos_a.x;
            tmp.y = (int) pos_a.y;
            /* enhance if wall */
            if(room->tiles[(int) tmp.y][(int) tmp.x].type == WALL) {
                view_get_absolute_position(&tmp, &tmp);
                MLV_draw_filled_rectangle(
                        tmp.x,
                        tmp.y,
                        SIDE, SIDE,
                        MLV_COLOR_RED
                );
            }
            view_get_absolute_position(&pos_a, &pos_a);
            MLV_draw_filled_circle(pos_a.x,pos_a.y,3,MLV_COLOR_GREEN);
        }
    }

    for(i = 0; i < ROOM_WIDTH; i++){
        xa = (i - p1->x) / (p2->x - p1->x);
        if(xa >= 0 && xa <= 1){ /* check if on segment */
            pos_a.x = i;
            position_interpolate_with_y(p1, p2, &pos_a);
            tmp.x = (int) pos_a.x;
            tmp.y = (int) pos_a.y;
            /* enhance if wall */
            if(room->tiles[(int) tmp.y][(int) tmp.x].type == WALL) {
                view_get_absolute_position(&tmp, &tmp);
                MLV_draw_filled_rectangle(
                        tmp.x,
                        tmp.y,
                        SIDE, SIDE,
                        MLV_COLOR_RED
                );
            }
            view_get_absolute_position(&pos_a, &pos_a);
            MLV_draw_filled_circle(pos_a.x,pos_a.y,3,MLV_COLOR_ORANGE);
        }
    }


}