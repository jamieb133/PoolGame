#pragma once

#include <raylib.h>

typedef struct {
    Rectangle outer, inner;
    bool enabled;
} ChargeBar;

void ChargeBar_Init(ChargeBar* bar, Vector2 pos, Vector2 size);
void ChargeBar_Draw(ChargeBar* bar, float charge);
