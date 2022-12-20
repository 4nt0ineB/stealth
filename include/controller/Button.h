/*
 *   This file is part of Stealth game
 *   Copyleft 2022 Yann ROUX--DAUGROIS
 *   and Antoine Bastos
 *   SPDX-License-Identifier: Apache-2.0
 */

#ifndef STEALTH_BUTTON_H
#define STEALTH_BUTTON_H

#include "core/geometry/Position.h"

#define BUTTON_LABEL_LEN 50

typedef struct button {
    unsigned char selected;
    double x, y;
    char label[BUTTON_LABEL_LEN + 1];
} Button;

void button_init(Button *button, const char *label, double x, double y);
void button_select(Button *button);
void button_unselect(Button *button);
int button_is_selected(const Button *button);
void button_unselect_all(Button *buttons, int n);

#endif /* STEALTH_BUTTON_H */
