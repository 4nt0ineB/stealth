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
#include <MLV/MLV_all.h>

/**
 * There is only one possible running window with MLV so why not use a global
 */
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
    Timer *timer;

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
void view_draw_info(View *view, const Room *room);

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
void draw_intersections_with_tiles(View *view, const Room *room, const Position *p1, const Position *p2);

#endif /* STEALTH_VIEW_H */
