#pragma once

#include <raylib.h>

typedef struct {
    Rectangle outer, inner;
    bool enabled;
    float charge, charge_vel;
} ChargeBar;

void ChargeBar_Init(ChargeBar* bar, Vector2 pos, Vector2 size, float charge_rate);
void ChargeBar_Draw(ChargeBar* bar);
