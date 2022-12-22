/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file View.h
 * @brief Implantation of the view for the game.
 * @details As the MLV lib is used and we can't create multiple
 * window. The data of the view is stored in a static variable View
 * and modified across the provided function in this module.
 *
 */

#ifndef STEALTH_VIEW_H
#define STEALTH_VIEW_H

#include "core/geometry/Rectangle.h"
#include "model/Room.h"
#include "core/Timer.h"
#include "controller/GameData.h"
#include "Button.h"
#include <MLV/MLV_all.h>

typedef enum {
    IMAGE_WALL
    , IMAGE_EMPTY
    , IMAGE_RELIC
    , IMAGE_BG
} ImageType ;

typedef struct {
    int fullscreen;
    int width;
    int height;
    int side;
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
    MLV_Font *font_text;
    MLV_Font *font_title;
    MLV_Image *images[IMAGE_BG + 1];
} View;

void view_draw_menu(View *view, const Button *buttons, int n);

/**
 * Init the window and the view
 */
void view_init(View *view);

/**
 * Some printing when dev
 */
void view_draw_util(View *view);

/**
 * @brief Draw the room
 * @param w
 * @param h
 */
void view_draw_room(View *view, const Room *room);

/**
 * Resize the window
 * and adapt the content
 * @param w
 * @param h
 */
void view_update_size(View *view, int w, int h);

void view_to_fullscreen(View *view);

void view_to_windowed(View *view);

int view_is_fullscreen(View *view);

/**
 * Draw the info area
 */
void view_draw_info(View *view, const GameData *data);

/**
 * close the view
 * and free its allocated properties
 */
void view_free(View *view);

/**
 * Draw a rectangle on the window
 * @param rectangle
 * @param color
 */
void draw_rectangle(View *view, const Rectangle *rectangle, MLV_Color color);

/**
 * Just for fun and debugging
 * @param room
 * @param p1
 * @param p2
 */
void draw_intersections_with_tiles(View *view, const Room *room, const Position *p1, const Position *p2);

void view_draw_guards(View *view, const GameData *data);
void view_draw_guard(View *view, const Guard *guard);
void view_draw_relics(View *view, const Relic *relics);
void view_draw_player(View *view, const Player *player);

/**
 * Draw a input box with the given title for the user to
 * write something
 * @param view
 * @param title
 * @param len max size input
 * @param dest string destination
 */
void view_ask_string(View *view,
                     const char *title,
                     int len,
                     char *dest);

/**
 * Draw the score board
 * By top mana and by top times
 *
 * @param view
 * @param data
 * @param scores_mana
 * @param nmana
 * @param scores_time
 * @param ntime
 */
void view_draw_score_board(const View *view,
                           const GameData *data,
                           const Score *scores_mana,
                           int nmana,
                           const Score *scores_time,
                           int ntime);

/**
 * Display the end game screen with
 * corresponding message
 * @param view
 * @param data
 * @param win
 */
void view_draw_end_msg(const View *view, const GameData *data, int win);

void view_draw_button(const View *view, const Button *button);

void view_get_button_size(const View *view, const Button *button, int *width, int *height);

#endif /* STEALTH_VIEW_H */
