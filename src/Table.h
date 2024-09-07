#pragma once

#include <raylib.h>

typedef struct {
    int x, y;
    float radius;
} Pocket;

typedef struct {
    Rectangle felt;
    Rectangle rail_upper;
    Rectangle rail_lower;
    Rectangle rail_left;
    Rectangle rail_right;
    Rectangle bottom_left_connector;
    Rectangle bottom_right_connector;
    Rectangle top_left_connector;
    Rectangle top_right_connector;
    Pocket pockets[6];
    float friction_coeff;
} Table;

void Table_Init(Table* table, int screen_width, int screen_height);
void Table_Draw(Table* table);