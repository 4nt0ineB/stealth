/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file Settings.h
 * @brief some globals and macros needed for the game
 *
 */

#ifndef STEALTH_SETTINGS_H
#define STEALTH_SETTINGS_H

/* Frames per second */
#define FPS 60
/* Square per second (v) */
#define SPS 10
#define PLAYER_INIT_SPEED 0.1
#define PLAYER_INCR_SPEED 0.03
#define PLAYER_MAX_SPEED 0.9
#define COMPUTE_MOVE_DIST(v) (((v) * SPS) / FPS)

#define ROOM_HEIGHT 45
#define ROOM_WIDTH 60

#define GUARD_NUMBER 5
#define GUARD_NEW_DIR_PROBABILITY 0.02
#define GUARD_MIN_SPEED 0.3
#define GUARD_MAX_SPEED 0.8
#define GUARD_VIEW_RANGE 4
#define GUARD_PANIC_SPEED 1
#define GUARD_PANIC_VIEW_RANGE 6


/**
 * Global of the size of a Tile
 * determined by the View
 * with the window dimensions.
 * See view_update_size().
 * Declared in Setting.c
 * */
extern int SIDE;

/*********************
 * GUI configuration *
 *********************/
/* Various sizes defined as percentage */
/* Window */
#ifndef DEFAULT_WIN_W_PERCENT
#define DEFAULT_WIN_W_PERCENT 70
#endif

#ifndef DEFAULT_WIN_H_PERCENT
#define DEFAULT_WIN_H_PERCENT 70
#endif

/* Info area */
#define DEFAULT_INFO_W_PERCENT 100
#define DEFAULT_INFO_H_PERCENT 10

/* Font */
/* Font height percent of window height */
#define FONT_HEIGHT_PERCENT 2

/*********************
 * Resources *
 *********************/
#ifndef RESOURCES_DIR
#define RESOURCES_DIR "../resources/"
#endif

#define RESOURCE(path_to_resource) RESOURCES_DIR path_to_resource

#endif /* STEALTH_SETTINGS_H */
