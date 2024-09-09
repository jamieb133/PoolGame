#include "ChargeBar.h"
#include <assert.h>
#include <stdlib.h>

void ChargeBar_Init(ChargeBar* bar, Vector2 pos, Vector2 size, float charge_rate)
{
    bar->enabled = false;
    bar->charge = 0.0f;
    bar->charge_vel = charge_rate;

    bar->outer.height = size.y;
    bar->outer.width = size.x;
    bar->outer.x = pos.x;
    bar->outer.y = pos.y;

    bar->inner.height = size.y;
    bar->inner.width = size.x;
    bar->inner.x = pos.x;
    bar->inner.y = pos.y;
}

void ChargeBar_Draw(ChargeBar* bar)
{
    assert(NULL != bar);
    assert(1.0f >= bar->charge);
    assert(0.0f <= bar->charge);

    if (!bar->enabled)
    {
        return;
    }

    DrawRectangleRec(bar->outer, BLACK);

    // Draw inner bar based on charge.
    float const width = bar->inner.width;
    bar->inner.width *= bar->charge;

    // Make color of the charge hotter for harder shots.
    if (bar->charge > 0.66f)
    {
        DrawRectangleRec(bar->inner, RED);
    }
    else if (bar->charge > 0.33f)
    {
        DrawRectangleRec(bar->inner, ORANGE);
    }
    else if (bar->charge > 0.0f)
    {
        DrawRectangleRec(bar->inner, BLUE);
    }

    // Reset height.
    bar->inner.width = width;
}