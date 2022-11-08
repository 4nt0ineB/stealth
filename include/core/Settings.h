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
/* Square per second */
#define SPS 1


#define ROOM_HEIGHT 45
#define ROOM_WIDTH 60

#define GUARD_NUMBER 5

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
