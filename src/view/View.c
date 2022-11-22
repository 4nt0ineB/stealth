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


void view_draw_info(Room *room){
    static char buffer[40] = {0};
    draw_rectangle(&View.info_area, MLV_COLOR_GRAY77);
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
    draw_rectangle(&View.available_area, MLV_COLOR_DARK_RED);
    sprintf(buffer, "%d fps", MLV_get_frame_rate());
    MLV_draw_text_with_font(View.info_area.w - View.info_area.w / 5
                            , View.info_area.origin.y + View.info_area.h / 3
                            , buffer
                            , View.font
                            ,MLV_COLOR_BLACK);
}

void view_draw_room(Room *room){
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
            switch (room->room[i][j].type) {
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
    MLV_draw_filled_circle(
            ox + room->player.position.x
            , oy + room->player.position.y
            , SIDE / 2
            , MLV_COLOR_RED);

    /* Guards */
    for(i = 0; i < GUARD_NUMBER; i++){
        MLV_draw_filled_circle(
                ox + room->guards[i].x
                , oy + room->guards[i].y
                , SIDE / 2
                , MLV_COLOR_BLUE);

        MLV_draw_circle(
                ox + room->guards[i].x
                , oy + room->guards[i].y
                , SIDE * 4 /* @Todo: to change. The guard must have a vision range */
                , MLV_COLOR_BLUE);
    }
}

void view_free(){
    MLV_free_font(View.font);
    MLV_free_window();
}

void draw_rectangle(Rectangle *rectangle, MLV_Color color){
    MLV_draw_filled_rectangle(
            rectangle->origin.x
            , rectangle->origin.y
            , rectangle->w
            , rectangle->h
            , color
    );
}