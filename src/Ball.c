#include "Ball.h"
#include "Table.h"
#include <stdlib.h>
#include <assert.h>
#include <raylib.h>
#include <raymath.h>

static void HandleBallCollision(Ball* a, Ball* b)
{
    Vector2 pos_a = { a->x, a->y };
    Vector2 pos_b = { b->x, b->y };
    Vector2 vel_a = { a->vx, a->vy };
    Vector2 vel_b = { b->vx, b->vy };

    Vector2 collision_normal = Vector2Subtract(pos_b, pos_a);
    float distance = Vector2Length(collision_normal);

    // Normalise the collision normal.
    Vector2 normal = Vector2Scale(collision_normal, 1.0f / distance);

    // Calculate relative velocity.
    Vector2 relative_velocity = Vector2Subtract(vel_b, vel_a);

    // Dot product to get velocity along the normal.
    float velocity_along_normal = Vector2DotProduct(relative_velocity, normal);

    if (velocity_along_normal > 0)
    {
        // Balls moving apart, no collision.
        return;
    }

    // Simple elastic collision response.
    // TODO: tune this
    float restitution = 0.2f;
    float impulse_scalar = -(1.0f + restitution) * velocity_along_normal;

    // Apply the impulse.
    Vector2 impulse = Vector2Scale(normal, impulse_scalar);
    a->vx -= impulse.x;
    a->vy -= impulse.y;
    b->vx += impulse.x;
    b->vy += impulse.y;

    // TODO: handle ball overlap
}

void Ball_Init(Ball* ball, int x, int y)
{
    ball->x = x;
    ball->y = y;
    ball->vx = 0;
    ball->vy = 0;
    ball->enabled = true;
}

void Ball_CheckCushionCollision(Ball* ball, Table* table)
{
    int x_min = table->felt.x;
    int x_max = table->felt.x + table->felt.width;
    int y_min = table->felt.y;
    int y_max = table->felt.y - table->felt.height;

    if ((ball->x - ball->radius <= x_min)
        && (ball->vx < 0)) // Don't reverse velocity if already travelling away.
    {
        // Reverse X velocity.
        ball->vx *= -1; 
    }

    if ((ball->x + ball->radius >= x_max)
        && (ball->vx > 0)) // Don't reverse velocity if already travelling away.
    {
        // Reverse X velocity.
        ball->vx *= -1; 
    }

    if ((ball->y - ball->radius <= y_min)
        && (ball->vy < 0)) // Don't reverse velocity if already travelling away.
    {
        // Reverse Y velocity.
        ball->vy *= -1;
    }

    if ((ball->y + ball->radius >= y_max)
        && (ball->vy > 0)) // Don't reverse velocity if already travelling away.
    {
        // Reverse Y velocity.
        ball->vy *= -1;
    }
}

void Ball_CheckBallCollision(Ball* a, Ball* b)
{
    assert(NULL != a);
    assert(NULL != b);

    if (a == b)
    {
        // Same ball, do nothing.
        return;
    }

    Vector2 pos_a = { a->x, a->y };
    Vector2 pos_b = { b->x, b->y };

    if (Vector2Distance(pos_a, pos_b) < (a->radius + b->radius))
    {
        HandleBallCollision(a, b);
    }
}

void Ball_ApplyFriction(Ball* ball, Table* table)
{
    ball->vx *= (1 - table->friction_coeff);
    ball->vy *= (1 - table->friction_coeff);

    // Stop the ball once it gets too slow.

    if (fabs(ball->vx) < 0.005)
    {
        ball->vx = 0;
    }

    if (fabs(ball->vy) < 0.005)
    {
        ball->vy = 0;
    }
}

void Ball_Update(Ball* ball, float dt)
{
    if (!ball->enabled)
    {
        return;
    }

    // Update position based on current velocity.
    ball->x += ball->vx * dt;
    ball->y += ball->vy * dt;

    DrawCircle(
        ball->x,
        ball->y,
        ball->radius,
        ball->color
    );
}