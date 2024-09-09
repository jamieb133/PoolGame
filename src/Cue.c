#include "Cue.h"
#include <math.h>

static float cue_offset_ = 25.0f;
static float rotation_speed_ = 45.0f; // degs per sec

static Rectangle RectangleRotationTransform(Rectangle rect, Vector2 rotation_point, float angle)
{
    float radians = DEG2RAD * angle;
    float cos_theta = cos(radians);
    float sin_theta = sin(radians);
    float trans_x = rect.x - rotation_point.x;
    float trans_y = rect.y - rotation_point.y;

    return (Rectangle) {
        .x = rotation_point.x + trans_x * cos_theta - trans_y * sin_theta,
        .y = rotation_point.y + trans_x * sin_theta + trans_y * cos_theta,
        .height = rect.height,
        .width = rect.width
    };
}

void Cue_Init(Cue* cue, Vector2 pos, Vector2 size)
{
    // TODO: fix size

    // Cue is 90deg to the left so height and width swapped.
    cue->shaft.width = 150;// = size.y;
    cue->shaft.height = 5;//= size.x;
    cue->shaft.x = pos.x + cue_offset_; // TODO: refine this
    cue->shaft.y = pos.y ;//- (cue->shaft.width / 2.0f);

    // TODO: place other sections of the cue relative the the shaft.
    // From top: tip => shaft => butt

    // Reset the chargebar location.
    Vector2 chargebar_loc = { pos.x, pos.y + 25.0f/*TODO scaled offset*/ };
    Vector2 chargebar_size = { 25, 50 };//TODO
    ChargeBar_Init(&cue->charge_bar, chargebar_loc, chargebar_size);  
}

void Cue_Draw(Cue* cue, float dt)
{
    // TODO: update pos based on vel

    // TODO: based on time not frame
    static float charge = 0.0f;
    //charge = (charge >= 1.0f) ? charge - 0.01f : charge + 0.01f;

    Vector2 origin = {  0.0f, 0.0f };
    Rectangle trans_cue = RectangleRotationTransform(cue->shaft, (Vector2) { cue->shaft.x - cue_offset_, cue->shaft.y }, cue->rotation);

    DrawRectanglePro(trans_cue, origin, cue->rotation, DARKBROWN);
    ChargeBar_Draw(&cue->charge_bar, charge);
}

void Cue_RotateClockwise(Cue* cue, float dt)
{
    cue->rotation += rotation_speed_ * dt; 
}

void Cue_RotateAnticlockwise(Cue* cue, float dt)
{
    cue->rotation -= rotation_speed_ * dt; 
}

void Cue_EnableChargeBar(Cue* cue, bool enable)
{
    cue->charge_bar.enabled = enable;   
}