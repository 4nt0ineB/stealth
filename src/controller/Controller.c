

#include "view/View.h"
#include "controller/Controller.h"
#include "core/Timer.h"
#include <MLV/MLV_all.h>

static inline Direction get_direction();

int slealth_controller(Room *room) {
    int run = 1;
    Direction direction;
    MLV_Keyboard_button touche = MLV_KEYBOARD_NONE;
    MLV_Button_state state;
    view_init();
    while (run) {
        /* Display the current frame, sample function */
        view_update_time();
        view_draw_info(room);
        view_draw_util();
        view_draw_room(room);
        MLV_update_window();
        /* events */
        MLV_get_event(
                &touche, NULL, NULL, NULL, NULL,
                NULL, NULL, NULL,
                &state
        );
        if (!MLV_get_keyboard_state(MLV_KEYBOARD_x)){
            break;
        }
        room_move_player(room, get_direction());
        /*printf("%d %s \n", state, MLV_convert_keyboard_button_to_string(touche));*/
        /* move entities */
        /* collision detection and other game mechanism */
        MLV_delay_according_to_frame_rate();
    }
    view_free();
    return 0;
}


static inline Direction get_direction() {
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