/*
 *   This file is part of Stealth game
 *    Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include "view/View.h"
#include "core/Settings.h"
#include "core/Util.h"
#include "Controller.h"


#include <assert.h>
#include <math.h>

static void view_get_absolute_position(View *view, const Position *position, Position *result);
static void view_draw_player_skills_info(const View *view, const Player *player);
static void view_draw_mana_bar(View *view, const GameData *data);
static void view_draw_stolen_relics(View *view, int n);


void view_to_fullscreen(View *view){
    view_update_size(view, MLV_get_desktop_width(), MLV_get_desktop_height());
    MLV_enable_full_screen();
    view->fullscreen = 1;
}

void view_to_windowed(View *view){
    MLV_disable_full_screen();
    view_update_size(view, (MLV_get_desktop_width() * DEFAULT_WIN_W_PERCENT) / 100,
                     (MLV_get_desktop_height() * DEFAULT_WIN_H_PERCENT) / 100);
    view->fullscreen = 0;
}

int view_is_fullscreen(View *view){
    return view->fullscreen;
}

static void view_init_images(View *view){
    MLV_Image *tmp = MLV_load_image("resources/img/sheet.png");
    view->images[IMAGE_WALL] =
            MLV_copy_partial_image(tmp, 256, 32, 8, 8);
    view->images[IMAGE_EMPTY] =
            MLV_copy_partial_image(tmp, 110, 240, 9, 9);
    view->images[IMAGE_RELIC] =
            MLV_copy_partial_image(tmp, 144, 160, 8, 8);
    MLV_free_image(tmp);
    tmp = MLV_load_image("resources/img/bg.png");
    int imgw = MLV_get_image_width(tmp);
    int imgh = MLV_get_image_height(tmp);
    view->images[IMAGE_BG] = MLV_copy_partial_image(tmp, 0, 0, imgw / 3, imgh);
    MLV_resize_image(view->images[IMAGE_BG], 1920, 1080);
    MLV_free_image(tmp);
}

void view_init(View *view){
    rectangle_init(&view->info_area, 0, 0, 0, 0);
    view->available_area = view->game_area = view->info_area;
    view->side = 0;
    view->font_text = NULL;
    view->bg_color = MLV_COLOR_BLACK;
    MLV_create_window("Stealth", "", 1, 1);
    view->fullscreen = 0;
    /* Default frame rate */
    MLV_change_frame_rate( FPS );
    /* Set window dimension to default values */
    view_update_size(view, (MLV_get_desktop_width() * DEFAULT_WIN_W_PERCENT) / 100,
                     (MLV_get_desktop_height() * DEFAULT_WIN_H_PERCENT) / 100);
    view_init_images(view);
}

void view_update_size(View *view, int w, int h){
    assert(w > 0 && h > 0);
    /* Update font */
    if(view->font_text) MLV_free_font(view->font_text);
    if(view->font_title) MLV_free_font(view->font_title);
    view->font_text = MLV_load_font(
            RESOURCE("fonts/militech.ttf")
            , (w * FONT_HEIGHT_PERCENT) / 100);
    view->font_title = MLV_load_font(
            RESOURCE("fonts/militech.ttf")
            , (w * FONT_HEIGHT_PERCENT * 2) / 100);
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
    view->game_area.origin.x = view->available_area.origin.x
                               + (view->available_area.w - view->game_area.w) / 2;
    view->game_area.origin.y = view->available_area.origin.y
                               + (view->available_area.h - view->game_area.h) / 2;
    MLV_change_window_size(w, h);
    view->width = MLV_get_window_width();
    view->height = MLV_get_window_height();
    if(view->images[IMAGE_BG])
        MLV_resize_image(view->images[IMAGE_BG], view->width, view->height);
}

void view_draw_menu(View *view, const Button *buttons, int n){
    int i;
    int txtbw, txtbh;
    MLV_Color bgcolor = MLV_rgba(29, 33, 54, 0);
    MLV_Color color = MLV_rgba(84, 174, 179, 0);
    /* bg */
    /*MLV_draw_filled_rectangle(0, 0, view->width, view->height, bgcolor);*/
    MLV_draw_image(view->images[IMAGE_BG], 0, 0);
    /* title */
    MLV_get_size_of_adapted_text_box_with_font("Stealth", view->font_title, 5, &txtbw, &txtbh);
    MLV_draw_adapted_text_box_with_font(view->width / 2 - txtbw / 2, view->height * 0.25,
                                        "Stealth", view->font_title, 5,
                                        bgcolor, color, bgcolor,
                                        MLV_TEXT_CENTER);
    /* buttons */
    for(i = 0; i < n; i++) view_draw_button(view, &buttons[i]);
}

void view_draw_info(View *view, const GameData *data){
    static char buffer[40] = {0};
    draw_rectangle(view, &view->info_area, 0);
    /* * * * * * * * *
     * Draw timer *
     * * * * * * * * */
    timer_sprintf(data->timer, buffer);

    MLV_draw_text_with_font(
            view->info_area.w / 2 - 50
            , view->info_area.origin.y + view->info_area.h / 3
            , buffer
            , view->font_text
            ,MLV_COLOR_DARK_GREY);
    /* mana / relics / skills */
    view_draw_mana_bar(view, data);
    view_draw_stolen_relics(view, controller_stolen_relic_count(data));
    view_draw_player_skills_info(view, &data->player);
}

void view_draw_util(View *view){
    static char buffer[10] = {0};
    /* draw framerate */
    /* draw_rectangle(view, &view->available_area, view->bg_color); */
    sprintf(buffer, "%d fps", MLV_get_frame_rate());
    MLV_draw_text_with_font(view->available_area.w - view->available_area.w / 10
            , view->available_area.h - view->available_area.h / 10
            , buffer
            , view->font_text
            ,MLV_COLOR_ORANGE);
}

void view_draw_player(View *view, const Player *player){
    Position pos;
    MLV_Color color = MLV_rgba(0, 99, 80, 255);
    unsigned char r,g,b,a;
    view_get_absolute_position(view, &player->position, &pos);
    if(skill_is_activated(player_skill(player, SPEED))){
        color = MLV_rgba(0, 231, 211,  255);
    }
    if(skill_is_activated(player_skill(player, INVISIBILITY))){
        MLV_convert_color_to_rgba(color, &r, &g, &b, &a);
        color = MLV_rgba(r, g, b, 127);
    }
    MLV_draw_filled_circle(pos.x, pos.y, view->side / 2, color);
}

void view_draw_relic(View *view, const Relic *relic){
    Position pos;
    MLV_Color color = relic_is_stolen(relic) ? MLV_rgba(0, 0, 0, 255) : MLV_COLOR_GREEN3;
    view_get_absolute_position(view, &relic->position, &pos);
    MLV_draw_filled_rectangle(pos.x, pos.y, view->side, view->side, color);
    MLV_draw_rectangle(
            pos.x
            , pos.y
            , view->side
            , view->side
            , MLV_rgba(224, 224, 0, 255)
    );
    if(!relic_is_stolen(relic) && view->images[IMAGE_RELIC]){
        MLV_resize_image_with_proportions(view->images[IMAGE_RELIC], view->side, view->side);
        MLV_draw_image(view->images[IMAGE_RELIC], pos.x, pos.y);
    }
}

void view_draw_guard(View *view, const Guard *guard){
    Position pos;
    MLV_Color color;
    color = MLV_rgba(164, 26, 94, 255);
    if(guard->panic_mode) color = MLV_rgba(255, 75, 250, 255);
    view_get_absolute_position(view, &guard->position, &pos);
    MLV_draw_filled_circle(
            pos.x
            , pos.y
            , view->side / 2
            , color);

    MLV_draw_circle(
            pos.x
            , pos.y
            , view->side * guard_view_range(guard) /* @Todo: to change. The guard must have a vision range */
            , color);
}

void view_draw_relics(View *view, const Relic *relics){
    int i;
    for(i = 0; i < RELICS_NUMBER; i++) view_draw_relic(view, &relics[i]);
}

void view_draw_guards(View *view, const GameData *data){
    double alpha;
    int i;
    /* Guards */
    for(i = 0; i < GUARD_NUMBER; i++)
        view_draw_guard(view, &data->guards[i]);
    /* show panic mode with red filter */
    if(guard_is_panicking(&data->guards[0])){
        /* We use the clock as a way of modulate the aplha
         * of the filter */
        /* cos^2  ->  [0, 1] */
        alpha = cos(data->timer->end.tv_sec);
        alpha = alpha * alpha * 70;
        MLV_draw_filled_rectangle(0, 0, view->width, view->height,
                                  MLV_rgba(255,0,0, alpha));
    }
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
    draw_rectangle(view, &view->game_area, MLV_rgba(61, 72, 91,255));
    MLV_draw_image(view->images[IMAGE_BG], 0, 0);

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
    MLV_Image *img_tile;
    for(i = 0; i < ROOM_HEIGHT; i++){
        for(j = 0; j < ROOM_WIDTH; j++){
            img_tile = NULL;
            pos.x = j;
            pos.y = i;
            view_get_absolute_position(view, &pos, &pos);
            /*if(room_get_tile_type(room, i, j) == EMPTY) continue;*/
            switch (room_get_tile_type(room, i, j)) {
                case WALL:
                    color = MLV_COLOR_BLACK;
                    img_tile = view->images[IMAGE_WALL];
                    break;
                case MANA:
                    color = MLV_rgba(0, 162, 184,120);
                    img_tile = view->images[IMAGE_EMPTY];
                    break;
                case EMPTY:
                    color = MLV_COLOR_BLACK;
                    break;
                default:
                    color = MLV_rgba(0,0,0,0);
            }
            MLV_draw_filled_rectangle(pos.x, pos.y, view->side, view->side, color);
            if(img_tile){
                MLV_resize_image_with_proportions(img_tile, view->side, view->side);
                MLV_draw_image(img_tile, pos.x, pos.y);
            }
        }
    }
    /* spawn */
    pos.x = 1;
    pos.y = 1;
    int spawn_side = view->side * 2;
    view_get_absolute_position(view, &pos, &pos);
    MLV_draw_filled_rectangle(pos.x, pos.y,
                              spawn_side, spawn_side,
                              MLV_rgba(111, 0, 255, 50));
}

void view_free(View *view){
    MLV_free_font(view->font_text);
    MLV_free_font(view->font_title);
    int i;
    for(i = 0; i < IMAGE_BG + 1; i++)
        MLV_free_image(view->images[i]);
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

    /* draw intersections */
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
            if(room_get_tile_type(room, tmp.y, tmp.x) == WALL) {
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
            if(room_get_tile_type(room, tmp.y, tmp.x) == WALL) {
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

void view_ask_string(View *view, const char *title, int len, char *dest){
    char *input;
    size_t inputlen;
    int boxw = (int) view->available_area.w / 5;
    int boxh = (int) view->available_area.h / 5;
    int boxx = (view->available_area.w / 2) - (boxw / 2);
    int boxy = (view->available_area.h / 2) - (boxh / 2);
    MLV_Color color = MLV_rgba(88, 174, 184, 255);
    MLV_Color bgcolor = MLV_rgba(29, 33, 54, 200);
    MLV_wait_input_box(
            boxx
            , boxy
            , boxw
            , boxh
            , bgcolor
            , color
            , bgcolor
            , title
            , &input
            );
    inputlen = strlen(input);
    memccpy(dest, input, -1, MIN(inputlen, len - 1));
    dest[MIN(inputlen, len - 1) + 1] = '\0';
    free(input);
}

void view_draw_score_board_impl(const View *view,
                                int x,
                                int y,
                                const char *title,
                                const Score *scores,
                                int n,
                                void (*score_to_string)(const Score *score, char *buff)){
    static char buffer[800];
    buffer[0] = '\0';
    int i;
    /* time */
    char tmpbuff[100] = {0};
    for(i = 0; i < n; i++){
        sprintf(tmpbuff, "%-20s", scores[i].name);
        strcat(buffer, tmpbuff);
        score_to_string(&scores[i], tmpbuff);
        strcat(buffer, tmpbuff);
        strcat(buffer, "\n");
    }
    int txtbw, txtbh;
    MLV_Color color = MLV_rgba(88, 174, 184, 255);
    MLV_Color bgcolor = MLV_rgba(29, 33, 54, 0);
    MLV_get_size_of_adapted_text_box(buffer, 5, &txtbw, &txtbh);
    MLV_draw_adapted_text_box(x - txtbw / 2, y,
                                        buffer, 5,
                                        bgcolor, color, bgcolor
            , MLV_TEXT_RIGHT);
    int txtw, txth;
    MLV_get_size_of_text_with_font(title, &txtw, &txth, view->font_text);
    MLV_draw_text_with_font(x - txtw / 2, y - txth * 2, title, view->font_text, color);
}

void view_draw_end_msg(const View *view, const GameData *data, int win) {
    /* Draw score recap */
    static char buffer[100];
    MLV_Font *font;
    MLV_Color bgcolor = MLV_rgba(29, 33, 54, 0);
    /*MLV_draw_filled_rectangle(0, 0, view->width, view->height, bgcolor);*/
    MLV_draw_image(view->images[IMAGE_BG], 0, 0);

    /* compute proper text */
    if (win) {
        sprintf(buffer, "Room accomplished in ");
        timer_sprintf(data->timer, buffer + 21);
        sprintf(buffer + strlen(buffer), "\n%d mana consumed\npress r to retry", data->score.mana);
        font = view->font_text;
    } else {
        sprintf(buffer, "You have failed\npress r to retry");
        font = view->font_title;
    }

    /* Display text */
    int txtbw, txtbh;
    MLV_Color color = MLV_rgba(212, 152, 42, 255);
    MLV_get_size_of_adapted_text_box_with_font(buffer, font, 5, &txtbw, &txtbh);
    MLV_draw_adapted_text_box_with_font(view->width / 2 - txtbw / 2, view->height * 0.10,
                                        buffer, font, 5,
                                        bgcolor, color, bgcolor, MLV_TEXT_CENTER);
}

void view_draw_score_board(const View *view,
                           const GameData *data,
                           const Score *scores_mana,
                           int nmana,
                           const Score *scores_time,
                           int ntime){
    /* time */
    view_draw_score_board_impl(view, view->width / 4, view->height / 3,
                               "Best times",
                               scores_time, ntime, score_sprintf_time);
    /* mana */
    view_draw_score_board_impl(view, view->width - view->width / 4, view->height / 3,
                               "Best mana consumption",
                               scores_mana, nmana, score_sprintf_mana);
}

void view_get_button_size(const View *view, const Button *button, int *width, int *height){
    int boxw, boxh;
    MLV_get_size_of_adapted_text_box_with_font(
            button->label
            , view->font_text
            , 5
            , &boxw
            , &boxh
    );
    if(width) *width = boxw;
    if(height) *height = boxh;
}

void view_draw_button(const View *view, const Button *button){
    MLV_Color bgcolor = MLV_rgba(29, 33, 54, 0);
    MLV_Color textcolor = MLV_rgba(212, 152, 42, 255);
    MLV_Color bordercolor = MLV_rgba(18, 20, 33, 0);
    int w, h;
    view_get_button_size(view, button, &w, &h);
    if(button_is_selected(button)){
        bgcolor = MLV_rgba(48, 54, 89, 255);
    }
    MLV_draw_adapted_text_box_with_font(
            button->x * view->width - w / 2
            , button->y * view->height - h / 2
            , button->label
            , view->font_text
            , 5
            , bordercolor
            , textcolor
            , bgcolor
            , MLV_TEXT_CENTER
            );
}

/***********
 *
 * Static
 *
 ***********/

static void view_get_absolute_position(View *view, const Position *position, Position *result){
    result->x = view->game_area.origin.x + position->x * view->side;
    result->y = view->game_area.origin.y + position->y * view->side;
}

static void view_draw_player_skills_info(const View *view, const Player *player){
    static char buffer[100] = {0};
    buffer[0] = '\0';
    int txtw;
    if(skill_is_activated(player_skill(player, INVISIBILITY))){
        strcat(buffer, "Invisible");
    }
    if(skill_is_activated(player_skill(player, SPEED))){
        strcat(buffer, " Speed");
    }
    MLV_get_size_of_text_with_font(buffer, &txtw, NULL, view->font_text);
    MLV_draw_text_with_font(view->info_area.w - txtw - view->info_area.w * 0.1
            , view->info_area.h * 0.1
            , buffer
            ,  view->font_text
            , MLV_COLOR_ORANGE);
}

static void view_draw_stolen_relics(View *view, int n){
    static char buffer[40] = {0};
    /* * * * * * * * * * * *
     * Draw stolen relics  *
     * * * * * * * * * * * */
    double square_size = view->info_area.h / 2.0;
    Rectangle relic = {
            .origin = {
                    .x = view->info_area.origin.x
                         + view->info_area.w * 0.1,
                    .y = view->info_area.origin.y
                         + view->info_area.h / 2 - square_size / 2
            },
            .h = square_size,
            .w = square_size
    };
    MLV_draw_filled_rectangle( relic.origin.x
            , relic.origin.y
            , relic.w
            , relic.h
            , MLV_COLOR_GREEN3);
    MLV_Image *tmp = MLV_copy_image(view->images[IMAGE_RELIC]);
    MLV_resize_image_with_proportions(tmp, relic.w, relic.h);
    MLV_draw_image(tmp, relic.origin.x, relic.origin.y);
    /* Draw the number of stolen relics */
    sprintf(buffer, "x %d", n);
    MLV_draw_text_with_font(relic.origin.x + relic.w +  view->info_area.w * 0.01
            , relic.origin.y + relic.h - relic.h * 0.8
            , buffer
            , view->font_text
            ,MLV_COLOR_WHITE);
}

static void view_draw_mana_bar(View *view, const GameData *data){
    static char buffer[40] = {0};
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
    double percentage = (MIN(data->player.mana, max_mana) * 100.0) / max_mana;
    MLV_draw_filled_rectangle(mana_quantity.origin.x
            , mana_quantity.origin.y
            , (mana_quantity.w * percentage) / 100.0
            , mana_quantity.h
            , MLV_COLOR_CYAN2);
    sprintf(buffer, "%d", data->player.mana);
    /* draw number of mana */
    MLV_draw_text_with_font(mana_quantity.origin.x + view->info_area.h * 0.1
            , mana_quantity.origin.y - mana_quantity.h * 0.1
            , buffer
            , view->font_text
            ,MLV_COLOR_DARK_GREY);

    /* draw "mana" label */
    int text_w;
    MLV_get_size_of_text_with_font("Mana", &text_w, NULL, view->font_text);
    MLV_draw_text_with_font(mana_quantity.origin.x - text_w - view->info_area.h * 0.1
            , mana_quantity.origin.y
            , "Mana"
            , view->font_text
            ,MLV_COLOR_WHITE);
}
