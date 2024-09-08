#pragma once 

#include <raylib.h>
#include "Table.h"

// TODO: convert dist and vel to vector2.
typedef struct {
    int x, y;                       // XY position
    float vx, vy;                   // XY velocity
    float radius;
    Color color;
    bool enabled;
} Ball;

// Initialise a stationary ball at the given coordinates.
void Ball_Init(Ball* ball, int x, int y);

bool Ball_CheckIfPocketed(Ball* ball, Table* table);

// Apply required velocity change if ball has hit a cushion.
void Ball_CheckCushionCollision(Ball* ball, Table* table);

// Apply required velocity change if ball has hit another ball.
void Ball_CheckBallCollision(Ball* ball_a, Ball* ball_b);

// Apply required velocity change according to the table's friction.
void Ball_ApplyFriction(Ball* ball, Table* table);

// Redraw the ball with updated position.
void Ball_Update(Ball* ball, float dt);

static inline bool Ball_IsMoving(Ball* ball)
{
    bool moving = true;

    moving &= ball->vx != 0;
    moving |= ball->vy != 0;
    moving &= ball->enabled;

    return moving;
}