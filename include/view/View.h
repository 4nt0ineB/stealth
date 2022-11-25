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
#include <MLV/MLV_all.h>

/**
 * Init the window and the view for the given dimensions
 */
void view_init();

/**
 * Some printing when dev
 */
void view_draw_util();

/**
 * @brief Draw the room
 * @param w
 * @param h
 */
void view_draw_room(const Room *room);

/**
 * Resize the window
 * and adapt the content
 * @param w
 * @param h
 */
void view_update_size(int w, int h);

/**
 * Draw the info area
 */
void view_draw_info(const Room *room);

/**
 * close the view
 * and free its allocated properties
 */
void view_free();

/**
 * Draw a rectangle on the window
 * @param rectangle
 * @param color
 */
void draw_rectangle(const Rectangle *rectangle, MLV_Color color);

/**
 * Update the elapsed time
 */
void view_update_time();

void draw_intersections_with_tiles(const Room *room, const Position *p1, const Position *p2);

#endif /* STEALTH_VIEW_H */
