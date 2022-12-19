
#include "model/Room.h"
#include "controller/Controller.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int args, char *argv[]){
    int ret_val;
    srand(time(NULL));
    while (1){
        ret_val = controller_menu();
        if (ret_val == -1) exit(EXIT_FAILURE);
        else if(ret_val == 0) break;
        /* No error and wants to retry so we continue the while loop*/
    }
    
    return ;
}