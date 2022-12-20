/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_GAMEDATA_H
#define STEALTH_GAMEDATA_H
#include "core/Settings.h"
#include "model/Room.h"
#include "core/Timer.h"

#include "model/Score.h"
#include "MLV/MLV_all.h"

typedef struct game_data {
    Player player;
    Score score;
    Relic relics[RELICS_NUMBER];
    Guard guards[GUARD_NUMBER];
    Room room;
    Timer *timer;
    MLV_Music *music_room;
    MLV_Music *music_alarm;
    MLV_Music *music_menu;
    MLV_Sound *sound_relic;
    MLV_Sound *sound_mana;
}GameData;

#endif /* STEALTH_GAMEDATA_H */
