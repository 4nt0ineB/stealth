/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include "view/View.h"
#include "core/Settings.h"
#include "core/Util.h"


#include <assert.h>
#include <math.h>


void view_init(View *view){
    rectangle_init(&view->info_area, 0, 0, 0, 0);
    view->available_area = view->game_area = view->info_area;

    view->timer = NULL;
    view->side = 0;
    view->font = NULL;
    view->bg_color = MLV_COLOR_BLACK;
    MLV_create_window("Slealth", "", 1, 1);

    /* Default frame rate */
    MLV_change_frame_rate( FPS );
    /* Set window dimension to default values */
    view_update_size(view, (MLV_get_desktop_width() * DEFAULT_WIN_W_PERCENT) / 100,
                     (MLV_get_desktop_height() * DEFAULT_WIN_H_PERCENT) / 100);
    MLV_draw_filled_rectangle(
            0
            , 0
            , MLV_get_window_width()
            , MLV_get_window_height()
            , view->bg_color);
    view->timer = new_timer();
    timer_start(view->timer);
}


void view_draw_info(View *view, const Room *room){
    static char buffer[40] = {0};
    draw_rectangle(view, &view->info_area, MLV_COLOR_GRAY50);
    /* * * * * * * * *
     * Draw timer *
     * * * * * * * * */
    timer_sprintf(view->timer, buffer);
    /* * * * * * * * *
     * Draw timer    *
     * * * * * * * * */
    MLV_draw_text_with_font(
            view->info_area.w / 2
            , view->info_area.origin.y + view->info_area.h / 3
            , buffer
            , view->font
            ,MLV_COLOR_BLACK);

    /* * * * * * * * *
     * Draw mana bar *
     * * * * * * * * */
    int bar_w = view->info_area.w * 0.1;
    int bar_h = view->info_area.h * 0.3;
    Rectangle mana_bar = {
        .origin = {
                .x = view->info_area.w - bar_w - view->info_area.w * 0.02,
                .y = view->info_area.h - bar_h - view->info_area.h * 0.1
        },
        .w = bar_w,
        .h = bar_h
    };
    /* mana bar background */
    MLV_draw_filled_rectangle(mana_bar.origin.x
                              , mana_bar.origin.y
                              , mana_bar.w
                              , mana_bar.h
                              , MLV_COLOR_BLACK);
    /* mana bar quantity */
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
    MLV_draw_text_with_font(mana_quantity.origin.x + view->info_area.h * 0.1
            , mana_quantity.origin.y - mana_quantity.h * 0.1
            , buffer
            , view->font
            ,MLV_COLOR_DARK_GREY);

    /* draw "mana" label */
    int text_w;
    MLV_get_size_of_text_with_font("Mana", &text_w, NULL, view->font);
    MLV_draw_text_with_font(mana_quantity.origin.x - text_w - view->info_area.h * 0.1
            , mana_quantity.origin.y
            , "Mana"
            , view->font
            ,MLV_COLOR_WHITE);

    /* * * * * * * * * * * *
     * Draw stolen relics  *
     * * * * * * * * * * * */
    double square_size = view->info_area.h / 2.0;
    Rectangle relic = {
            .origin = {
                    .x = view->info_area.origin.x + view->info_area.w * 0.1,
                    .y = view->info_area.origin.y + view->info_area.h / 2 - square_size / 2
            },
            .h = square_size,
            .w = square_size
    };
    MLV_draw_filled_rectangle( relic.origin.x
            , relic.origin.y
            , relic.w
            , relic.h
            , MLV_COLOR_GREEN3);
    /* Draw the number of stolen relics */
    sprintf(buffer, "x %d", room_stolen_relic_count(room));
    MLV_draw_text_with_font(relic.origin.x + relic.w +  view->info_area.w * 0.01
            , relic.origin.y + relic.h - relic.h * 0.8
            , buffer
            , view->font
            ,MLV_COLOR_WHITE);
}

void view_update_time(View *view){
    timer_update(view->timer);
}

void view_draw_util(View *view){
    static char buffer[10] = {0};
    /* draw framerate */
    draw_rectangle(view, &view->available_area, MLV_rgba(41, 52, 71,255));
    sprintf(buffer, "%d fps", MLV_get_frame_rate());
    MLV_draw_text_with_font(view->available_area.w - view->available_area.w / 10
            , view->available_area.h - view->available_area.h / 10
            , buffer
            , view->font
            ,MLV_COLOR_ORANGE);
}

void view_update_size(View *view, int w, int h){
    assert(w > 0 && h > 0);
    /* Update font */
    if(view->font)
        MLV_free_font(view->font);
    view->font = MLV_load_font(
            RESOURCE("fonts/militech.ttf")
            , (w * FONT_HEIGHT_PERCENT) / 100);
    /* Info area */
    view->info_area.w = (w * DEFAULT_INFO_W_PERCENT) / 100;
    view->info_area.h = (h * DEFAULT_INFO_H_PERCENT) / 100;
    /* Available area, for the calculation of the game area dimensions  */
    view->available_area.origin.y = view->info_area.h;
    view->available_area.w = w;
    view->available_area.h = h - view->info_area.h;
    /* Compute view->side from the new available area dimensions  */
    view->side = greatest_common_multiplier_less_than(
            ROOM_WIDTH,
            ROOM_HEIGHT,
            MIN(view->available_area.w, view->available_area.h));
    view->side = view->side / MIN(ROOM_WIDTH, ROOM_HEIGHT);
    /* Game area */
    view->game_area.w = view->side * ROOM_WIDTH;
    view->game_area.h = view->side * ROOM_HEIGHT;
    /* Center the game area in the available area */
    view->game_area.origin.x = view->available_area.origin.x + (view->available_area.w - view->game_area.w) / 2;
    view->game_area.origin.y = view->available_area.origin.y + (view->available_area.h - view->game_area.h) / 2;
    MLV_change_window_size(w, h);
}



static void view_get_absolute_position(View *view, const Position *position, Position *result){
    result->x = view->game_area.origin.x + position->x * view->side;
    result->y = view->game_area.origin.y + position->y * view->side;
}

void view_draw_player(View *view, const Player *character){
    Position pos;
    view_get_absolute_position(view, &character->position, &pos);
    MLV_draw_filled_circle(pos.x, pos.y, view->side / 2, MLV_COLOR_RED);
}

void view_draw_relic(View *view, const Relic *relic){
    Position pos;
    MLV_Color color = relic->stolen ? MLV_rgba(0, 0, 0, 0) : MLV_COLOR_GREEN3;
    view_get_absolute_position(view, &relic->position, &pos);
    MLV_draw_filled_rectangle(pos.x, pos.y, view->side, view->side, color);
    MLV_draw_rectangle(
            pos.x
            , pos.y
            , view->side
            , view->side
            , MLV_rgba(224, 224, 0, 255)
    );

}

void view_draw_guard(View *view, const Guard *guard){
    Position pos;
    MLV_Color color;
    color = MLV_COLOR_BLUE;
    if(guard->panic_mode) color = MLV_COLOR_YELLOW;
    view_get_absolute_position(view, &guard->position, &pos);
    MLV_draw_filled_circle(
            pos.x
            , pos.y
            , view->side / 2
            , color);

    MLV_draw_circle(
            pos.x
            , pos.y
            , view->side * 4 /* @Todo: to change. The guard must have a vision range */
            , color);
}

void view_draw_room(View *view, const Room *room){
    /*
     * The absolute position of any object of the room to be drawn
     * is determined in the view !
     */
    Position pos;
    MLV_Color color;

    int i, j;
    /* The Background */
    draw_rectangle(view, &view->game_area, MLV_COLOR_DARK_GREY);

    /* The Grid */
    char thickness = 1;
    /* Draw the lines */
    for(i = 0; i < ROOM_HEIGHT; i++){
        pos.x = 0;
        pos.y = i;
        view_get_absolute_position(view, &pos, &pos);
        MLV_draw_filled_rectangle(
                pos.x
                , pos.y
                , view->game_area.w
                , thickness
                , MLV_COLOR_BLACK);
    }
    /* Draw the columns */
    for(j = 0; j < ROOM_WIDTH; j++){
        pos.x = j;
        pos.y = 0;
        view_get_absolute_position(view, &pos, &pos);
        MLV_draw_filled_rectangle(
                pos.x
                , pos.y
                , thickness
                , view->game_area.h
                , MLV_COLOR_BLACK);
    }

    /* Draw the tiles : to delegate */
    for(i = 0; i < ROOM_HEIGHT; i++){
        for(j = 0; j < ROOM_WIDTH; j++){
            pos.x = j;
            pos.y = i;
            view_get_absolute_position(view, &pos, &pos);
            /*
             * @Todo delegate
             * */
            if(room->tiles[i][j].type == EMPTY) continue;
            switch (room->tiles[i][j].type) {
                case WALL:
                    color = MLV_COLOR_BLACK;
                    break;
                case MANA:
                    color = MLV_rgba(0, 162, 184,120);
                    break;
                case EMPTY:
                case RELIC:
                default:
                    color = MLV_rgba(0,0,0,0);
            }
            MLV_draw_filled_rectangle(pos.x, pos.y, view->side, view->side, color);
        }
    }


    for(i = 0; i < RELICS_NUMBER; i++){
        view_draw_relic(view, &room->relics[i]);
    }

    /* Player */
    view_draw_player(view, &room->player);

    /* Guards */
    for(i = 0; i < GUARD_NUMBER; i++){
        view_draw_guard(view, &room->guards[i]);
    }

    /* show panic mode with red filter */
    if(room->guards[0].panic_mode){
        /* We use the clock as a way of modulate the aplha
         * of the filter */
        /* cos^2  ->  [0, 1] */
        double alpha = cos(view->timer->end.tv_sec);
        alpha = alpha * alpha * 70;
        MLV_draw_filled_rectangle(0, 0, MLV_get_window_width(), MLV_get_window_height(),
                                  MLV_rgba(255,0,0, alpha));
    }
}

void view_free(View *view){
    MLV_free_font(view->font);
    MLV_free_window();
}

void draw_rectangle(View *view, const Rectangle *rectangle, const MLV_Color color){
    MLV_draw_filled_rectangle(
            rectangle->origin.x, rectangle->origin.y
            , rectangle->w, rectangle->h, color);
}


void draw_intersections_with_tiles(View *view, const Room *room, const Position *p1, const Position *p2){
    assert(p1 && p2);
    /* draw line between positions */
    Position abs_pos1, abs_pos2;
    view_get_absolute_position(view, p1, &abs_pos1);
    view_get_absolute_position(view, p2, &abs_pos2);
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
                view_get_absolute_position(view, &tmp, &tmp);
                MLV_draw_filled_rectangle(
                        tmp.x,
                        tmp.y,
                        view->side, view->side,
                        MLV_COLOR_RED
                );
            }
            view_get_absolute_position(view, &pos_a, &pos_a);
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
                view_get_absolute_position(view, &tmp, &tmp);
                MLV_draw_filled_rectangle(
                        tmp.x,
                        tmp.y,
                        view->side, view->side,
                        MLV_COLOR_RED
                );
            }
            view_get_absolute_position(view, &pos_a, &pos_a);
            MLV_draw_filled_circle(pos_a.x,pos_a.y,3,MLV_COLOR_ORANGE);
        }
    }
}