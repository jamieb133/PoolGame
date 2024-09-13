#include "Cue.h"
#include "Utils.h"
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

void Cue_Init(Cue* cue, Vector2 pos, Vector2 size, float charge_rate)
{
    // TODO: fix size

    cue->pos = pos;
    cue->origin = pos;
    cue->velocity = (Vector2){ 0, 0 };
    cue->acceleration = (Vector2){ 0, 0 };
    cue->prev_windtime = 0;

    // Cue is 90deg to the left so height and width swapped.
    cue->shaft.width = 150;// = size.y;
    cue->shaft.height = 5;//= size.x;
    cue->shaft.x = pos.x + cue_offset_; // TODO: refine this
    cue->shaft.y = pos.y ;//- (cue->shaft.width / 2.0f);

    // TODO: place other sections of the cue relative the the shaft.
    // From top: tip => shaft => butt

    // Reset the chargebar location.
    Vector2 chargebar_loc = { pos.x, pos.y + 25.0f/*TODO scaled offset*/ };
    Vector2 chargebar_size = { 50, 25 };//TODO
    ChargeBar_Init(&cue->charge_bar, chargebar_loc, chargebar_size, charge_rate);  
}

void Cue_Draw(Cue* cue, float dt)
{
    Vector2 origin = {  0.0f, 0.0f };

    // Apply velocity.
    cue->shaft.x += dt * cue->velocity.x;
    cue->shaft.y += dt * cue->velocity.y;

    Rectangle trans_cue = RectangleRotationTransform(cue->shaft, (Vector2) { cue->shaft.x - cue_offset_, cue->shaft.y }, cue->rotation);

    // Update position metadata.
    cue->pos.x = trans_cue.x;
    cue->pos.y = trans_cue.y;

    DrawRectanglePro(trans_cue, origin, cue->rotation, DARKBROWN);
    ChargeBar_Draw(&cue->charge_bar);
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

void Cue_UpdateCharge(Cue* cue, float dt)
{
    if ((cue->charge_bar.charge >= 1.0f)
        || (cue->charge_bar.charge <= 0.0f))    
    {
        cue->charge_bar.charge_vel *= -1.0f;
    }

    cue->charge_bar.charge += cue->charge_bar.charge_vel * dt;
    cue->charge_bar.charge = (cue->charge_bar.charge > 1.0f) ? 1.0f : cue->charge_bar.charge;
    cue->charge_bar.charge = (cue->charge_bar.charge < 0.0f) ? 0.0f : cue->charge_bar.charge;

    TraceLog(LOG_WARNING, "Charge is %f", cue->charge_bar.charge);
}

void Cue_SetVelocity(Cue* cue, float magnitude)
{
    cue->velocity = MagnitudeToVector(magnitude, cue->rotation);
}

bool Cue_CheckBallContact(Cue* cue, Ball* ball)
{
    // Find the closest point on the rectangle to the circle's center
    float closest_x = fmaxf(cue->shaft.x, fminf(ball->x, cue->shaft.x + cue->shaft.width));
    float closest_y = fmaxf(cue->shaft.y, fminf(ball->y, cue->shaft.y + cue->shaft.height));

    // Calculate the distance between the circle's center and this closest point
    float distance_x = ball->x - closest_x;
    float distance_y = ball->y - closest_y;

    // Calculate the squared distance (saves computing a square root for optimization)
    float distanceSquared = (distance_x * distance_x) + (distance_y * distance_y);

    if (distanceSquared > (ball->radius * ball->radius))
    {
        // Not touching, do nothing.
        return false;
    }

    // Apply impulse to the ball.
    ball->vx += cue->velocity.x;
    ball->vy += cue->velocity.y;

    // Start decelerating. TODO
   // cue->acceleration = 
    cue->velocity = (Vector2) {0,0};

    return true;
}