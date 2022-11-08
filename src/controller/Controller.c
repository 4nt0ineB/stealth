

#include "view/View.h"
#include "controller/Controller.h"
#include "core/Timer.h"
#include <MLV/MLV_all.h>

int slealth_controller(Room *room){
    int run = 1;
    view_init();
    while(run){
        /* Display the current frame, sample function */
        view_update_time();
        view_draw_info(room);
        view_draw_util();
        view_draw_room(room);
        MLV_update_window();
        /* events */
        /* move entities */
        /* collision detection and other game mechanism */
        MLV_delay_according_to_frame_rate();
    }
    MLV_wait_seconds(5);
    view_free();
    return 0;
}