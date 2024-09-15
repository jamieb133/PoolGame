#include <raylib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "GameContext.h"
#include "Ball.h"
#include "Utils.h"
#include "Logger.h"

// State handlers.
static void CueAimHandler(GameContext* context);
static void CueChargeHandler(GameContext* context);
static void CueStrikeHandler(GameContext* context);
static void BallMotionHandler(GameContext* context);
static void FinishedHandler(GameContext* context);

// State entry actions.
static void CueAimEntryAction(GameContext* context);
static void CueChargeEntryAction(GameContext* context);
static void CueStrikeEntryAction(GameContext* context);
static void BallMotionEntryAction(GameContext* context);
static void FinishedEntryAction(GameContext* context);

static StateTick state_handlers_[NUM_STATES] = {
    CueAimHandler,      // CUE_AIM
    CueChargeHandler,   // CUE_CHARGE
    CueStrikeHandler,   // CUE_STRIKE
    BallMotionHandler,  // BALL_MOTION
    FinishedHandler     // FINISHED
};

static StateAction state_entry_actions_[NUM_STATES] = {
    CueAimEntryAction,      // CUE_AIM
    CueChargeEntryAction,   // CUE_CHARGE
    CueStrikeEntryAction,   // CUE_STRIKE
    BallMotionEntryAction,  // BALL_MOTION
    FinishedEntryAction     // FINISHED
};

static const char* state_strings_[NUM_STATES] = {
    "CUE_AIM",      // CUE_AIM
    "CUE_CHARGE",   // CUE_CHARGE
    "CUE_STRIKE",   // CUE_STRIKE
    "BALL_MOTION",  // BALL_MOTION
    "FINISHED"      // FINISHED
};

static void TransitionTo(GameContext* context, GameState state)
{
    assert(NUM_STATES > state);
    TraceLog(LOG_INFO, "State transition %s --> %s", 
        state_strings_[context->state], state_strings_[state]);
    context->state = state;
    state_entry_actions_[state](context);
}

static inline void TickState(GameContext* context)
{
    state_handlers_[context->state](context);
}

static void ResetTurns(GameContext* context)
{
    Player* p1 = context->current_player;
    context->current_player = context->next_player;
    context->next_player = p1;
}

static void DrawTable(GameContext* context)
{
    Table_Draw(&context->table);
}

static void DrawBalls(GameContext* context)
{
    for (int i = 0; i < 16; i++)
    {
        Ball* ball = &context->balls[i];
        Ball_Update(ball, context->dt);
    }
}

static void DrawCue(GameContext* context)
{
    Cue_Draw(&context->cue, context->dt);
}

void GameContext_Init(GameContext* context, int const width, int const height)
{
    context->screen_size = (Vector2) { width, height };

    // Create the table to actually play the game on.
    Table_Init(&context->table, width, height);

    // Set radius of the balls and disable by default.
    for (int i = 0; i < 16; i++)
    {
        context->balls[i].enabled = false;
        context->balls[i].radius = 15.0f;
    }

    // Set the colours.
    context->black_ball = &context->balls[14];
    context->white_ball = &context->balls[15];
    context->balls[14].color = BLACK;
    context->balls[15].color = WHITE;

    for (int i = 0; i < 14; i++)
    {
        context->balls[i].color = ((i % 2) == 0) ? RED : YELLOW;
    }

    Ball* black = context->black_ball;
    Ball* white = context->white_ball;
    Ball* red_0 = &context->balls[0];
    Ball* yel_0 = &context->balls[1];
    Ball* red_1 = &context->balls[2];
    Ball* yel_1 = &context->balls[3];
    Ball* red_2 = &context->balls[4];
    Ball* yel_2 = &context->balls[5];
    Ball* red_3 = &context->balls[6];
    Ball* yel_3 = &context->balls[7];
    Ball* red_4 = &context->balls[8];
    Ball* yel_4 = &context->balls[9];
    Ball* red_5 = &context->balls[10];
    Ball* yel_5 = &context->balls[11];
    Ball* red_6 = &context->balls[12];
    Ball* yel_6 = &context->balls[13];
    Ball* red_7 = &context->balls[14];
    Ball* yel_7 = &context->balls[15];
    float diameter = black->radius * 2; // These are all the same.
    float radius = black->radius;

    // Set the initial positions.
    //TODO: for now just do the black and white balls.
    Ball_Init(black, width * 0.25, height / 2);
    Ball_Init(white, width * 0.75, height / 2);

    // Place the remaining red and yellow balls surrounding the black ball as expected.
    // Don't do anything clever just do them one by one, I can't be arsed.

    // First row.
    Ball_Init(red_0, black->x + (diameter * 2), black->y);

    // Second row.
    Ball_Init(yel_0, black->x + diameter, black->y - radius);
    Ball_Init(red_1, black->x + diameter, black->y + radius);

    // Place balls either side of black.
    Ball_Init(red_2, black->x, black->y - diameter); 
    Ball_Init(yel_1, black->x, black->y + diameter); 

    // Fourth row.
    Ball_Init(yel_2, black->x - diameter, black->y - (radius * 3));
    Ball_Init(red_3, black->x - diameter, black->y - radius);
    Ball_Init(yel_3, black->x - diameter, black->y + radius);
    Ball_Init(red_4, black->x - diameter, black->y + (radius * 3));

    // Fifth and final row.
    Ball_Init(red_5, black->x - (diameter * 2), black->y - (diameter * 2));
    Ball_Init(yel_4, black->x - (diameter * 2), black->y - diameter);
    Ball_Init(red_6, black->x - (diameter * 2), black->y);
    Ball_Init(yel_5, black->x - (diameter * 2), black->y + diameter);
    Ball_Init(yel_6, black->x - (diameter * 2), black->y + (diameter * 2));

    context->player_1.score = 0;
    context->player_2.score = 0;
    context->player_1.ball_color = RAYWHITE;
    context->player_2.ball_color = RAYWHITE;
    context->player_1.name = "Player 1";
    context->player_2.name = "Player 2";
    context->current_player = &context->player_1;
    context->next_player = &context->player_2;
    context->pocketed = 0;
    context->state = CUE_AIM;
    TransitionTo(context, CUE_AIM);
}

void GameContext_Update(GameContext* context, float const dt)
{
    context->dt = dt;
    ClearBackground(BLUE);

    // Draw game entities
    DrawTable(context);
    DrawCue(context);
    DrawBalls(context);

    // Tick state machine
    TickState(context);
}

void GameContext_Deinit(GameContext* context)
{
}

void GameContext_Reload(GameContext* context)
{
    // Reset any pointers to static/global data here
}

void CueAimEntryAction(GameContext* context)
{
    Cue_Init(
        &context->cue, 
        (Vector2) { context->white_ball->x, context->white_ball->y }, 
        (Vector2) { context->screen_size.x / 240.0f, context->screen_size.y / 8.0f },
        2.0f
    );
}

void CueAimHandler(GameContext* context)
{
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_W))
    {
        Cue_RotateClockwise(&context->cue, dt);
    }
    
    if (IsKeyDown(KEY_S))
    {
        Cue_RotateAnticlockwise(&context->cue, dt);
    }

    if (IsKeyDown(KEY_SPACE))
    {
        // Player is charging up a shot.
        TransitionTo(context, CUE_CHARGE);
    }
}

void CueChargeEntryAction(GameContext* context)
{
    Cue_EnableChargeBar(&context->cue, true);
}

void CueChargeHandler(GameContext* context)
{
    float dt = GetFrameTime();

    Cue_UpdateCharge(&context->cue, dt);

    if (!IsKeyDown(KEY_SPACE))
    {   
        TransitionTo(context, CUE_STRIKE);
    }
}

void CueStrikeEntryAction(GameContext* context)
{
    // Start winding cue back for shot.
    Cue_SetVelocity(&context->cue, 250.0f * context->cue.charge_bar.charge);
}

void CueStrikeHandler(GameContext* context)
{
    Cue* cue = &context->cue;

    // One second windup.
    cue->prev_windtime += context->dt;

    if (cue->prev_windtime >= 0.5f)
    {
        // Now start pushing the cue forward.
        Cue_SetVelocity(cue, -250.0f * cue->charge_bar.charge);
        cue->prev_windtime = 0.0f;
    }

    if (Cue_CheckBallContact(cue, context->white_ball))
    {
        // Ball contact, initiate ball motion state
        TransitionTo(context, BALL_MOTION);
    }
}

void BallMotionEntryAction(GameContext* context)
{
    // TODO
}

void BallMotionHandler(GameContext* context)
{
    float dt = GetFrameTime();
    bool in_motion = false;

    // Update ball physics.
    for (int i = 0; i < 16; i++)
    {
        Ball* ball = &context->balls[i];

        // Check if the ball was pocketed, kaching.
        if (Ball_CheckIfPocketed(ball, &context->table))
        {
            if (ColorCmp(ball->color, BLACK))
            {
                break;
            }

            if (ColorCmp(ball->color, context->current_player->ball_color))
            {
                context->current_player->score++;
            }
            else 
            {
                
                if (!ColorCmp(ball->color, WHITE))
                {
                    // Potted opponent's ball... :(
                    context->next_player->score++;
                }
            }
            continue;
        }

        // Check for collisions with walls of the table.
        Ball_CheckCushionCollision(ball, &context->table);

        // Check for collisions with other balls.
        for (int j = 0; j < 16; j++)
        {
            if (i == j)
                continue;
            Ball* ball2 = &context->balls[j];
            Ball_CheckBallCollision(ball, ball2);
        }

        // Must contextly some friction to stop the balls just whacking away indefinitely.
        Ball_ApplyFriction(ball, &context->table);

        // Prevent state transition until all balls have stopped.
        if (Ball_IsMoving(ball))
        {
            in_motion = true;
        }
    }

    // If all balls have stopped moving then prepare for the next shot.
    if (!in_motion)
    {
        TransitionTo(context, CUE_AIM);
    }
}

void FinishedEntryAction(GameContext* context)
{
    // TODO
}

void FinishedHandler(GameContext* context)
{
    // TODO
}