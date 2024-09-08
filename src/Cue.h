#pragma once

#include <raylib.h>

typedef struct {
    Rectangle shaft;
    Rectangle butt;
    Rectangle tip;
    Vector2 position;
} Cue;

void Cue_Init(Cue* cue);
void Cue_Draw(Cue* cue);

// Update the cue position such that it moves around the ball clockwise.
void Cue_YawLeft(Cue* cue);

// Update the cue position such that it moves around the ball anticlockwise.
void Cue_YawRight(Cue* cue);