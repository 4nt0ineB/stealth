

#include "view/View.h"
#include "controller/Controller.h"
#include "core/Timer.h"
#include "view/View.h"
#include "Button.h"

static int controller_in_spawn(int x, int y);

int controller_run(){
    View view;
    GameData data;
    view_init(&view);
    if(controller_init_audio(&data)) return 1;
    while(controller_menu(&view, &data))
        do{
            controller_init(&data);
        }while(controller_end_game(&view,&data,controller_game_loop(&view, &data)));
    view_free(&view);
    MLV_free_audio();
    return 0;
}

int controller_menu(View *view, GameData *data){
    MLV_Event event;
    MLV_Keyboard_modifier mod;
    MLV_Keyboard_button sym;
    MLV_Button_state state;
    MLV_Mouse_button mouse_button;
    int mousex, mousey;
    Button buttons[2];
    Button *selected;
    button_init(&buttons[0], "Fullscreen", 0.9, 0.1);
    button_init(&buttons[1], "Play", 0.5, 0.5);
    MLV_play_music(data->music_menu, 0.5f, -1);
    while(1){
        /* buttons state */
        button_unselect_all(buttons, 2);
        selected = NULL;
        selected = controller_pos_over_button(view, buttons,2, mousex, mousey);
        if(selected) button_select(selected);
        /* window */
        view_draw_menu(view, buttons, 2);
        MLV_update_window();
        /* events */
        MLV_get_mouse_position(&mousex, &mousey);
        event = MLV_get_event(&sym, &mod,
                              0,0, 0,
                              0, 0, &mouse_button,
                              &state);
        if(event == MLV_MOUSE_BUTTON
        && mouse_button == MLV_BUTTON_LEFT
        && state == MLV_PRESSED){
            if(button_is_selected(&buttons[0])){ /* windows size */
                if(view_is_fullscreen(view)){
                    view_to_windowed(view);
                    strcpy(buttons[0].label, "Fullscreen");
                }else{
                    view_to_fullscreen(view);
                    strcpy(buttons[0].label, "Windowed");
                }
            }
            if(button_is_selected(&buttons[1])){ /* play */
                return 1;
            }
        }
        if(event == MLV_KEY && state == MLV_PRESSED){
            if(sym == MLV_KEYBOARD_ESCAPE)
                return 0;
            else if(sym == MLV_KEYBOARD_RETURN){
                /* play */
                return 1;
            }
        }
        /* Cap refresh rate */
        MLV_delay_according_to_frame_rate();
    }
}

int controller_game_loop(View *view, GameData *data) {
    int detection_overview = 0;
    MLV_Keyboard_button touche = MLV_KEYBOARD_NONE;
    MLV_Button_state state;
    int mana;
    MLV_play_music(data->music_room, 0.5f, -1);
    int i, win, pause = 0;
    /* While user didn't press any move key we stay in pause state so 
    user can see the room and where the relics are ...*/
    while (!(win = controller_win(data))) {
        /* Display the current frame, sample function */
        if (!pause){
            controller_update_time(data);
            controller_check_player(data);
            if(controller_check_guards_find_player(data)){
                ; /* Player have been caught then display end game screen */
            }
            controller_check_guard_panic(data);
        }
        controller_update_view(data, view);
        if(detection_overview)
            for(i = 0; i < GUARD_NUMBER; i++){ /* just for fun and debugging */
                draw_intersections_with_tiles(view, &data->room
                                              , &data->player.position
                                              , &data->guards[i].position);
            }
        MLV_update_window();
        /* get keyboard events */
        MLV_get_event(&touche, NULL, NULL, NULL,
                      NULL,NULL, NULL, NULL,&state);
        /* quit with x */
        if (MLV_get_keyboard_state(MLV_KEYBOARD_p) == MLV_PRESSED){
            /* Make the game pause and unpause ?*/
            pause = pause ? 0 : 1;
        }
        if(!MLV_get_keyboard_state(MLV_KEYBOARD_ESCAPE))
            break;
        /* detection overview */
        if (!MLV_get_keyboard_state(MLV_KEYBOARD_o))
            detection_overview = !detection_overview;
        if (!pause){
            /* deactivate skill by default */
            player_deactivate_all_skills(&data->player);
            /* SPEED SKILL */
            if(!MLV_get_keyboard_state(MLV_KEYBOARD_RSHIFT)
            || !MLV_get_keyboard_state(MLV_KEYBOARD_LSHIFT)){
                skill_activate(player_skill(&data->player, SPEED));
            }
            /* INVISIBILITY SKILL */
            if(!MLV_get_keyboard_state(MLV_KEYBOARD_SPACE)){
                skill_activate(player_skill(&data->player, INVISIBILITY));
            }
            /* regenerate consumed mana in the room */
            mana = player_consume_mana(&data->player);
            room_add_mana(&data->room, mana);
            data->score.mana += mana;
            /* check if skill should still be activated depending on the players mana */
            player_update_skills_state(&data->player);
            /* Move entities */
            controller_move_player(data, get_direction_from_keyboard());
            controller_move_guards(data);
        }
        /* Cap refresh rate */
        MLV_delay_according_to_frame_rate();
    }
    data->score.time = timer_get_delta(data->timer);
    return win;
}

int controller_end_game(View *view, GameData *data, int win){
    MLV_Event event;
    MLV_Keyboard_modifier mod;
    MLV_Keyboard_button sym;
    MLV_Button_state state;
    Score scores_mana[SCORE_SAVED + 1] = {0};
    Score scores_time[SCORE_SAVED + 1] = {0};
    MLV_stop_music();
    MLV_play_music(data->music_menu, 0.5f, -1);
    controller_save_score(view, data);
    int nmana = score_read("resources/score_mana", scores_mana, SCORE_SAVED);
    int ntime = score_read("resources/score_time", scores_time, SCORE_SAVED);
    while(1){
        view_draw_end_msg(view, data, win);
        view_draw_score_board(view, data, scores_mana, nmana, scores_time, ntime);
        MLV_update_window();
        event = MLV_get_event(&sym, &mod,
                              0,0, 0,0, 0, 0,
                              &state);
        if(event == MLV_KEY && state == MLV_PRESSED){
            if(sym == MLV_KEYBOARD_ESCAPE)
                return 0;
            else if(sym == MLV_KEYBOARD_r){
                /* Retry */
                return 1;
            }
        }
        /* Cap refresh rate */
        MLV_delay_according_to_frame_rate();
    }
}

void controller_save_score(View *view, GameData *data){
    int i;
    int win = controller_win((const GameData *) &data->player);
    Score scores_mana[SCORE_SAVED + 1] = {0};
    Score scores_time[SCORE_SAVED + 1] = {0};
    for(i = 0; i < SCORE_SAVED; i++) scores_time[i].time = __LONG_MAX__;
    for(i = 0; i < SCORE_SAVED; i++) scores_mana[i].mana = __INT_MAX__;
    int nmana = score_read("resources/score_mana", scores_mana, SCORE_SAVED);
    int ntime = score_read("resources/score_time", scores_time, SCORE_SAVED);
    view_draw_score_board(view, data, scores_mana, nmana, scores_time, ntime);
    view_draw_end_msg(view, data, win);
    MLV_update_window();
    if(win){
        /* check if in top scores */
        int in_top_mana = !nmana
                || score_cmp_mana(&scores_mana[SCORE_SAVED - 1], &data->score) > 0;
        int in_top_time = !ntime
                || score_cmp_time(&scores_time[SCORE_SAVED - 1], &data->score) > 0;
        if(in_top_mana || in_top_time){
            /* ask name */
            view_ask_string(view, "Name: ", NAME_LENGTH, data->score.name);
            /* put in mana score board */
            if(in_top_mana){
                scores_mana[SCORE_SAVED] = data->score;
                qsort(scores_mana, SCORE_SAVED + 1, sizeof(Score), score_cmp_mana);
                score_write("resources/score_mana",
                            scores_mana, MIN(nmana + 1, SCORE_SAVED));
            }
            /* put in time score board */
            if(in_top_time){
                scores_time[SCORE_SAVED] = data->score;
                qsort(scores_time, SCORE_SAVED + 1, sizeof(Score), score_cmp_time);
                score_write("resources/score_time",
                            scores_time, MIN(ntime + 1, SCORE_SAVED));
            }
        }
    }
}

Button * controller_pos_over_button(const View *view, Button *buttons, int n, int x, int y){
    int i, bw, bh;
    for(i = 0; i < n; i++){
        view_get_button_size(view, buttons + i, &bw, &bh);
        if(x >= buttons[i].x * view->width - bw / 2
        && x <= buttons[i].x * view->width + bw / 2
        && y >= buttons[i].y * view->height - bh / 2
        && y <= buttons[i].y * view->height + bh / 2)
            return buttons + i;
    }
    return NULL;
}

void controller_update_view(GameData *data, View *view){
    view_draw_room(view, &data->room);
    /*view_draw_util(view);*/
    view_draw_info(view, data);
    view_draw_relics(view, data->relics);
    view_draw_guards(view, data);
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
    data->music_menu = MLV_load_music("resources/music/menu.ogg");
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
        do {
            room_random_position(&data->room, EMPTY, &x, &y);
        } while (x < 8 || y < 8);
        /* Guard can't spawn too close to player else they will see 
        him before player can even move */
        guard_init(&(data->guards[i]) , x, y);
    }
    /* mana */
    room_add_mana(&data->room, MANA_TILES_NUMBER);
    /* Relics */
    controller_init_relics(data);
    /* Time */
    data->timer = new_timer();
    timer_start(data->timer);
}

void entity_move(Position *position, double speed, Direction direction){
    assert(position);
    position->x += COMPUTE_MOVE_DIST(speed) * direction_factor[direction][0];
    position->y += COMPUTE_MOVE_DIST(speed) * direction_factor[direction][1];
}

void controller_move_player(GameData *data, Direction direction){
    assert(data);
    player_update_speed(&data->player, direction);
    entity_move(&data->player.position, data->player.speed, direction);
    if(room_resolve_collision(&data->room, &data->player.position)){
        /* collided */
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
        if (guard_is_panicking(&data->guards[i])){
            /* Then his panic count is reset */
            guard_reset_panic_count(&data->guards[i]);
        } else guard_panic(&data->guards[i]);
    }
}

int controller_check_guard_panic(GameData *data){
    int i, j;
    /* update all panic counters */
    int panicking = guard_is_panicking(&data->guards[0]);
    /* check if any guard see a missing relic */
    for(i = 0; i < GUARD_NUMBER; i++) {
        guard_update_panic_count(&data->guards[i]);
        for (j = 0; j < RELICS_NUMBER; j++) {
            if (controller_guard_sees_missing_relic(&data->room, &data->guards[i], &data->relics[j])) {
                /* the missing relic is being noticed by all guards now */
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
    /* No more panicking, then restart music */
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
        /* SPEED skill consume 2 mana per frame 
        and INVISIBILITY consume 1 mana per frame
        and 60 fps so player need 60 mana to speed for a single frame
        there's a lot of mana tiles also so :
        if tiles get 3 mana each tile 20 tiles = 1 second of speed or 2 second of invisibility seems reasonable*/
        data->player.mana += 3; /* Previous was 1 */
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
                && position_eq(&current_tile, &data->relics[i].position)){
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
        /* player in view range */
        if(position_dist(&data->guards[i].position, &data->player.position)
           < guard_view_range(&data->guards[i])
           /* player is visible*/
           && !skill_is_activated(player_skill(&data->player, INVISIBILITY))
           /* no wall between them */
           && !room_tile_between(&data->room, &data->guards[i].position, &data->player.position, WALL)){
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


int controller_win(const GameData *data){
    return
    controller_stolen_relic_count(data) == RELICS_NUMBER
    && controller_in_spawn(data->player.position.x, data->player.position.y);
    ;
}

void controller_update_time(GameData *data){
    timer_update(data->timer);
}

Direction get_direction_from_keyboard() {
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

int controller_guard_sees_missing_relic(const Room *room, const Guard *guard, const Relic *relic){
    if(relic->stolen && position_dist(&guard->position, &relic->position)
                       < guard_view_range(guard)
       && !room_tile_between(room, &guard->position, &relic->position, WALL)
       && !relic->noticed /* can't panic for seeing a stolen relic twice */
            ){
        return 1;
    }
    return 0;
}


static int controller_in_spawn(int x, int y){
    return x >= 1
           && x <= 3
           && y >= 1
           && y <= 3;
}