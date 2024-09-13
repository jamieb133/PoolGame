#pragma once

#include <raylib.h>
#include "ChargeBar.h"
#include "Ball.h"

typedef struct {
    Rectangle shaft, butt, tip;
    Vector2 origin, pos;
    float rotation;
    ChargeBar charge_bar;
    Vector2 velocity, acceleration;
    float prev_windtime; // TODO: refactor this
} Cue;

void Cue_Init(Cue* cue, Vector2 origin, Vector2 size, float charge_rate);
void Cue_Draw(Cue* cue, float dt);
void Cue_RotateClockwise(Cue* cue, float dt);
void Cue_RotateAnticlockwise(Cue* cue, float dt);
void Cue_EnableChargeBar(Cue* cue, bool enable);
void Cue_UpdateCharge(Cue* cue, float dt);
void Cue_SetVelocity(Cue* cue, float magnitude);
bool Cue_CheckBallContact(Cue* cue, Ball* ball);