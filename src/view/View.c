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

static int SIDE = 0;

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
    /* Draw timer */
    MLV_draw_text_with_font(
            View.info_area.w / 2
            , View.info_area.origin.y + View.info_area.h / 3
            , buffer
            , View.font
            ,MLV_COLOR_BLACK);

    /* Draw mana bar */
    int bar_w = View.info_area.w * 0.1;
    int bar_h = View.info_area.h * 0.3;
    Rectangle mana_bar = {
        .origin = {
                .x = View.info_area.w - bar_w - View.info_area.w * 0.02,
                .y = View.info_area.h - bar_h - View.info_area.h * 0.1
        },
        .w = bar_w,
        .h = bar_h
    };
    /* bar background */
    MLV_draw_filled_rectangle(mana_bar.origin.x
                              , mana_bar.origin.y
                              , mana_bar.w
                              , mana_bar.h
                              , MLV_COLOR_BLACK);
    /* mana quantity */
    Rectangle mana_quantity;
    rectangle_with_padding(&mana_bar, 0.02, 0.05, &mana_quantity);

    /* update to proportional quantity */
    double max_mana =  (MANA_TILES_NUMBER / 4); /* for ux design purpose only */
    double percentage = (MIN(room->player.mana, max_mana) * 100.0) / max_mana;
    MLV_draw_filled_rectangle(mana_quantity.origin.x
            , mana_quantity.origin.y
            , (mana_quantity.w * percentage) / 100.0
            , mana_quantity.h
            , MLV_COLOR_CYAN2);
    sprintf(buffer, "%d", room->player.mana);
    /* draw number of mana */
    MLV_draw_text_with_font(mana_quantity.origin.x + View.info_area.h * 0.1
            , mana_quantity.origin.y - mana_quantity.h * 0.1
            , buffer
            , View.font
            ,MLV_COLOR_DARK_GREY);

    /* draw "mana" label */
    int text_w;
    MLV_get_size_of_text_with_font("Mana", &text_w, NULL, View.font);
    MLV_draw_text_with_font(mana_quantity.origin.x - text_w - View.info_area.h * 0.1
            , mana_quantity.origin.y
            , "Mana"
            , View.font
            ,MLV_COLOR_WHITE);
}

void view_update_time(){
    timer_update(View.timer);
}

void view_draw_util(){
    static char buffer[10] = {0};
    /* draw framerate */
    draw_rectangle(&View.available_area, MLV_rgba(41, 52, 71,255));
    sprintf(buffer, "%d fps", MLV_get_frame_rate());
    MLV_draw_text_with_font(View.available_area.w - View.available_area.w / 10
            , View.available_area.h - View.available_area.h / 10
            , buffer
            , View.font
            ,MLV_COLOR_ORANGE);
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



static void view_get_absolute_position(const Position *position, Position *result){
    result->x = View.game_area.origin.x + position->x * SIDE;
    result->y = View.game_area.origin.y + position->y * SIDE;
}

void view_draw_player(const Character *character){
    Position pos;
    view_get_absolute_position(&character->position, &pos);
    MLV_draw_filled_circle(pos.x, pos.y, SIDE / 2, MLV_COLOR_RED);
}

void view_draw_relic(const Relic *relic){
    Rectangle tile;
    Position pos;
    MLV_Color color = relic->taken ? MLV_rgba(0, 0, 0, 0) : MLV_COLOR_SALMON;
    view_get_absolute_position(&relic->position, &pos);
    MLV_draw_filled_rectangle(pos.x, pos.y, SIDE, SIDE, color);
    MLV_draw_rectangle(
            pos.x
            , pos.y
            , SIDE
            , SIDE
            , MLV_rgba(224, 224, 0, 255)
    );

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
     * is determined in the view !
     */
    Position pos;

    int i, j;
    /* The Background */
    draw_rectangle(&View.game_area, MLV_COLOR_DARK_GREY);

    /* The Grid */
    char thickness = 1;
    /* Draw the lines */
    for(i = 0; i < ROOM_HEIGHT; i++){
        pos.x = 0;
        pos.y = i;
        view_get_absolute_position(&pos, &pos);
        MLV_draw_filled_rectangle(
                pos.x
                , pos.y
                , View.game_area.w
                , thickness
                , MLV_COLOR_BLACK);
    }
    /* Draw the columns */
    for(j = 0; j < ROOM_WIDTH; j++){
        pos.x = j;
        pos.y = 0;
        view_get_absolute_position(&pos, &pos);
        MLV_draw_filled_rectangle(
                pos.x
                , pos.y
                , thickness
                , View.game_area.h
                , MLV_COLOR_BLACK);
    }

    /* Draw the tiles : to delegate */
    for(i = 0; i < ROOM_HEIGHT; i++){
        for(j = 0; j < ROOM_WIDTH; j++){
            pos.x = j;
            pos.y = i;
            view_get_absolute_position(&pos, &pos);
            /*
             * @Todo delegate
             * */
            switch (room->tiles[i][j].type) {
                case WALL:
                    MLV_draw_filled_rectangle(
                            pos.x
                            , pos.y
                            , SIDE
                            , SIDE
                            , MLV_COLOR_BLACK
                            );
                    break;
                case MANA:
                    MLV_draw_filled_rectangle(
                            pos.x
                            , pos.y
                            , SIDE
                            , SIDE
                            , MLV_rgba(0, 162, 184,120)
                    );
                    break;
                case EMPTY:
                    break;
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

    for(i = 0; i < RELICS_NUMBER; i++){
        view_draw_relic(&room->relics[i]);
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
    Position pos_a, tmp;

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