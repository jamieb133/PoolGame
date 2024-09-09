#include "ChargeBar.h"
#include <assert.h>
#include <stdlib.h>

void ChargeBar_Init(ChargeBar* bar, Vector2 pos, Vector2 size)
{
    bar->enabled = false;
    
    bar->outer.height = size.y;
    bar->outer.width = size.x;
    bar->outer.x = pos.x;
    bar->outer.y = pos.y;

    // TODO: place inner in center of outer
    bar->inner.height = size.y * 0.9f;
    bar->inner.width = size.x * 0.9f;
    bar->inner.x = pos.x;
    bar->inner.y = pos.y;
}

void ChargeBar_Draw(ChargeBar* bar, float charge)
{
    assert(NULL != bar);
    assert(1.0f >= charge);
    assert(0.0f <= charge);

    if (!bar->enabled)
    {
        return;
    }

    DrawRectangleRec(bar->outer, BLACK);

    // Draw inner bar based on charge.
    bar->inner.height *= charge;

    // Make color of the charge hotter for harder shots.
    if (charge > 0.66f)
    {
        DrawRectangleRec(bar->inner, RED);
    }
    else if (charge > 0.33f)
    {
        DrawRectangleRec(bar->inner, ORANGE);
    }
    else if (charge > 0.0f)
    {
        DrawRectangleRec(bar->inner, BLUE);
    }
}