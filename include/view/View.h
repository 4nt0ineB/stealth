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
#include "Controller.h"
#include <MLV/MLV_all.h>

typedef enum {
    IMAGE_WALL
    , IMAGE_EMPTY
    , IMAGE_RELIC
} ImageType ;

typedef struct {
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
    MLV_Font *font;
    MLV_Image *images[IMAGE_RELIC + 1];
} View;

/**
 * Init the window and the view for the given dimensions
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
 * Update the elapsed time
 */
void view_update_time(View *view);

/**
 * Just for fun and debugging
 * @param room
 * @param p1
 * @param p2
 */
void draw_intersections_with_tiles(View *view,
                                   const Room *room,
                                   const Position *p1,
                                   const Position *p2);

void view_draw_guards(View *view, const GameData *data);
void view_draw_relics(View *view, const Relic *relics);
void view_draw_player(View *view, const Player *player);

void view_ask_string(View *view,
                     const char *title,
                     int len,
                     char *dest);

void view_draw_score_board(const View *view,
                           const GameData *data,
                           const Score *scores_mana,
                           int nmana,
                           const Score *scores_time,
                           int ntime);

void view_draw_menu(View *view,
                    const char **choices,
                    const char *enhanced_choice);

void view_draw_end_msg(const View *view, const GameData *data, int win);

#endif /* STEALTH_VIEW_H */
