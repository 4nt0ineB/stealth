/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#include "Button.h"
#include <string.h>
#include <assert.h>

void button_init(Button *button, const char *label, double x, double y){
    assert(button);
    assert(strlen(label) < BUTTON_LABEL_LEN);
    button->x = x;
    button->y = y;
    strcpy(button->label, label);
    button_unselect(button);
}

void button_select(Button *button){
    assert(button);
    button->selected = 1;
}

void button_unselect(Button *button){
    assert(button);
    button->selected = 0;
}

int button_is_selected(const Button *button){
    assert(button);
    return button->selected;
}

void button_unselect_all(Button *buttons, int n){
    assert(buttons);
    int i;
    for(i = 0; i < n; i++)
        button_unselect(buttons + i);
}