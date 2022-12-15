

#include "view/View.h"
#include "controller/Controller.h"
#include "core/Timer.h"


static Direction get_direction_from_keyboard();
static void controller_update_view(GameData *data, View *view);

int slealth_controller() {
    int detection_overview = 0;
    MLV_Keyboard_button touche = MLV_KEYBOARD_NONE;
    MLV_Button_state state;
    View view;
    GameData data;
    controller_init(&data);
    view_init(&view);
    if(controller_init_audio(&data)){
        return 1;
    }
    MLV_play_music(data.music_room, 0.5f, -1);
    int i;
    while (1) {
        /* Display the current frame, sample function */
        view_update_time(&view);
        controller_check_player(&data);
        if(controller_check_guards_find_player(&data)){
            ; /* Player have been caught then display end game screen */
        }
        controller_check_guard_panic(&data);
        controller_update_view(&data, &view);
        if(detection_overview)
            for(i = 0; i < GUARD_NUMBER; i++){ /* just for fun and debugging */
                draw_intersections_with_tiles(&view, &data.room, &data.player.position, &data.guards[i].position);
            }
        MLV_update_window();
        /* get keyboard events */
        MLV_get_event(&touche, NULL, NULL, NULL,
                      NULL,NULL, NULL, NULL,&state);
        /* quit with x */
        if (!MLV_get_keyboard_state(MLV_KEYBOARD_x))
            break;
        if (!MLV_get_keyboard_state(MLV_KEYBOARD_o))
            detection_overview = !detection_overview;
        player_deactivate_all_skills(&data.player);
        if(!MLV_get_keyboard_state(MLV_KEYBOARD_RSHIFT)
           || !MLV_get_keyboard_state(MLV_KEYBOARD_LSHIFT)){
            skill_activate(player_skill(&data.player, SPEED));
        }
        if(!MLV_get_keyboard_state(MLV_KEYBOARD_SPACE)){
            skill_activate(player_skill(&data.player, INVISIBILITY));
        }
        room_add_mana(&data.room, player_consume_mana(&data.player));
        player_update_skills_state(&data.player);
        /* Move entities */
        controller_move_player(&data, get_direction_from_keyboard());
        controller_move_guards(&data);
        /* Cap refresh rate */
        MLV_delay_according_to_frame_rate();
    }
    view_free(&view);
    MLV_free_audio();
    return 0;
}

static void controller_update_view(GameData *data, View *view){
    view_draw_info(view, data);
    view_draw_util(view);
    view_draw_room(view, &data->room);
    view_draw_relics(view, data->relics);
    view_draw_guards(view, data->guards);
    view_draw_player(view, &data->player);

}

void controller_init_relics(GameData *data){
    int i, posx, posy;
    Position pos;
    for (i = 0; i < RELICS_NUMBER; i++){
        do {
            posx = int_rand(1, ROOM_WIDTH - 1);
            posy = int_rand(1, ROOM_HEIGHT - 1);
        }while((posx < 3 && posy < 3)
               || room_get_tile_type(&data->room, posy, posx) != EMPTY);
        tile_set_type(
                room_get_tile(&data->room, posy, posx)
                , RELIC);
        position_init(&pos, posx, posy);
        init_relic(&data->relics[i], pos);
    }
}

int controller_init_audio(GameData *data){
    if( MLV_init_audio() ){
        fprintf(stderr,"Could not load audio lib.");
        return 1;
    }
    data->music_room = MLV_load_music("resources/music/stealth.ogg");
    data->music_alarm = MLV_load_music("resources/music/alarm.ogg");
    data->sound_mana = MLV_load_sound("resources/music/mana.ogg");
    data->sound_relic = MLV_load_sound("resources/music/relic.ogg");
    return 0;
}

void controller_init(GameData *data){
    room_init(&data->room);
    /* player */
    player_init(&(data->player), 2, 2);
    /* guards */
    int i, x, y;
    for(i = 0; i < GUARD_NUMBER; i++){
        room_random_position(&data->room, EMPTY, &x, &y);
        guard_init(&(data->guards[i]) , x, y);
    }
    /* mana */
    room_add_mana(&data->room, MANA_TILES_NUMBER);
    /* Relics */
    controller_init_relics(data);
}

/**
 * Moves the entity by a given Velocity (speed and direction)
 * @param position
 * @param speed
 * @param direction
 */
void entity_move(Position *position, double speed, Direction direction){
    assert(position);
    position->x += COMPUTE_MOVE_DIST(speed) * direction_factor[direction][0];
    position->y += COMPUTE_MOVE_DIST(speed) * direction_factor[direction][1];
}

void controller_move_player(GameData *data, Direction direction){
    assert(data);
    player_update_speed(&data->player, direction);
    entity_move(&data->player.position, data->player.speed, direction);
    if(room_resolve_collision(&data->room, &data->player.position)){  /* collided */
        /* The player is hitting the wall, so the speed must be reset to init speed */
        player_update_speed(&data->player, STILL);
    }
}

void controller_move_guards(GameData *data){
    assert(data);
    int i;
    for(i = 0; i < GUARD_NUMBER; i++){
        guard_update_speed(&data->guards[i]);
        entity_move(&data->guards[i].position
                , data->guards[i].speed
                , guard_update_direction(&data->guards[i]));
        if(room_resolve_collision(&data->room, &data->guards[i].position)){ /* collided */
            /* The guard is hitting the wall, so the speed and the direction must be updated again */
            guard_update_speed(&data->guards[i]);
            guard_update_direction(&data->guards[i]);
        }
    }
}

void controller_make_guards_panic(GameData *data){
    int i;
    for(i = 0; i < GUARD_NUMBER; i++){
        if (data->guards[i].panic_mode){
            /* Then his panic count is reset */
            guard_reset_panic_count(&data->guards[i]);
        } else guard_panic(&data->guards[i]);
    }
}

/**
 *
 * @param room
 * @param guard
 * @param relic
 * @return
 */
int controller_guard_sees_missing_relic(const Room *room, const Guard guard, const Relic relic){
    if(relic.stolen && position_dist(&guard.position, &relic.position)
                       < guard_view_range(&guard)
       && !room_tile_between(room, &guard.position, &relic.position, WALL)
       && !relic.noticed /* can't panic for seeing a stolen relic twice*/
            ){
        return 1;
    }
    return 0;
}

int controller_check_guard_panic(GameData *data){
    int i, j;
    /* update all panic counters */
    int panicking = guard_is_panicking(&data->guards[0]);
    for(i = 0; i < GUARD_NUMBER; i++) {
        guard_update_panic_count(&data->guards[i]);
        for (j = 0; j < RELICS_NUMBER; j++) {
            if (controller_guard_sees_missing_relic(&data->room, data->guards[i], data->relics[j])) {
                /* the missing relic has been noticed by all guards now */
                data->relics[j].noticed = 1;
                /* Play alarm sound if not already panicking */
                if(!guard_is_panicking(&data->guards[i])){
                    MLV_stop_music();
                    MLV_play_music(data->music_alarm, 1.5f, -1);
                }
                /* every guard goes panicking */
                controller_make_guards_panic(data);
                return 1;
            }
        }
    }
    /* No more panicking then restart music */
    if(panicking != guard_is_panicking(&data->guards[0])){
        MLV_stop_music();
        MLV_play_music(data->music_room, 0.5f, -1);
    }
    return 0;
}

void controller_check_player(GameData *data){
    int i;
    Position current_tile = {
            .x = (int) data->player.position.x,
            .y = (int) data->player.position.y
    };
    Tile *tile = room_get_tile(&data->room, (int) current_tile.y, (int) current_tile.x);
    /* Take the mana on the tile if exists */
    if(tile->type == MANA){
        data->player.mana += 1;
        tile->type = EMPTY;
        /* mana sound */
        MLV_play_sound(data->sound_mana, 0.1f);
    }
    /* Look if he is on a relic tile and the relic isn't taken so he takes it*/
    if (tile->type == RELIC){
        /* Player is on a relic Tile */
        /* Lets look wich one */
        for (i = 0; i < RELICS_NUMBER; i++){
            /* No need to calculate pos if the relic is taken even if not the right one */
            if (!data->relics[i].stolen
                && is_same_position(&current_tile, &data->relics[i].position)){
                /* It's this relic and relic not taken */
                relic_steal(&data->relics[i]);
                /* relic steal sound */
                MLV_play_sound(data->sound_relic, 0.5f);
            }
        }
    }
}

int controller_check_guards_find_player(GameData *data){
    int i;
    for(i = 0; i < GUARD_NUMBER; i++){
        if(position_dist(&data->guards[i].position, &data->player.position)
           < guard_view_range(&data->guards[i])
           && !room_tile_between(&data->room, &data->guards[i].position,
                                 &data->player.position, WALL)){
            return 1;
        }
    }
    return 0;
}

int controller_stolen_relic_count(const GameData *data){
    int i, count;
    for(i = count = 0; i < RELICS_NUMBER; i++)
        if(relic_is_stolen(&data->relics[i])) count++;
    return count;
}

static Direction get_direction_from_keyboard() {
    static Direction direction;
    if (!MLV_get_keyboard_state(MLV_KEYBOARD_z)
        && !MLV_get_keyboard_state(MLV_KEYBOARD_d))
        direction = NE;
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_s) == MLV_PRESSED
             && MLV_get_keyboard_state(MLV_KEYBOARD_d) == MLV_PRESSED)
        direction = SE;
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_s) == MLV_PRESSED
             && MLV_get_keyboard_state(MLV_KEYBOARD_q) == MLV_PRESSED)
        direction = SO;
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_z) == MLV_PRESSED
             && MLV_get_keyboard_state(MLV_KEYBOARD_q) == MLV_PRESSED)
        direction = NO;
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_z) == MLV_PRESSED)
        direction = N;
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_q) == MLV_PRESSED)
        direction = O;
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_s) == MLV_PRESSED)
        direction = S;
    else if (MLV_get_keyboard_state(MLV_KEYBOARD_d) == MLV_PRESSED)
        direction = E;
    else direction = STILL;
    return direction;
}