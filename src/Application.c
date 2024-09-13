#include <raylib.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "Application.h"
#include "Ball.h"
#include "Utils.h"
#include "Logger.h"

// State handlers.
static void CueAimHandler(struct Application* app);
static void CueChargeHandler(struct Application* app);
static void CueStrikeHandler(struct Application* app);
static void BallMotionHandler(struct Application* app);
static void FinishedHandler(struct Application* app);

// State entry actions.
static void CueAimEntryAction(struct Application* app);
static void CueChargeEntryAction(struct Application* app);
static void CueStrikeEntryAction(struct Application* app);
static void BallMotionEntryAction(struct Application* app);
static void FinishedEntryAction(struct Application* app);

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

static void TransitionTo(Application* app, GameState state)
{
    assert(NUM_STATES > state);
    TraceLog(LOG_INFO, "State transition %s --> %s", 
        state_strings_[app->state], state_strings_[state]);
    app->state = state;
    app->state_handler = state_handlers_[state];
    state_entry_actions_[state](app);
}

static void TickState(Application* app)
{
    app->state_handler(app);
}

static void ResetTurns(Application* application)
{
    Player* p1 = application->current_player;
    application->current_player = application->next_player;
    application->next_player = p1;
}

static void DrawTable(Application* app)
{
    Table_Draw(&app->table);
}

static void DrawBalls(Application* app, float dt)
{
    for (int i = 0; i < 16; i++)
    {
        Ball* ball = &app->balls[i];
        Ball_Update(ball, dt);
    }
}

static void DrawCue(Application* app, float dt)
{
    Cue_Draw(&app->cue, dt);
}

void Application_Init(Application* application, int screen_width, int screen_height, const char* name)
{
    // Set custom logger function.
    Logger_Init();

    application->name = name;
    application->screen_width = screen_width;
    application->screen_height = screen_height;

    // Create the table to actually play the game on.
    Table_Init(&application->table, screen_width, screen_height);

    // Set radius of the balls and disable by default.
    for (int i = 0; i < 16; i++)
    {
        application->balls[i].enabled = false;
        application->balls[i].radius = 15.0f;
    }

    // Set the colours.
    application->black_ball = &application->balls[14];
    application->white_ball = &application->balls[15];
    application->balls[14].color = BLACK;
    application->balls[15].color = WHITE;

    for (int i = 0; i < 14; i++)
    {
        application->balls[i].color = ((i % 2) == 0) ? RED : YELLOW;
    }

    Ball* black = application->black_ball;
    Ball* white = application->white_ball;
    Ball* red_0 = &application->balls[0];
    Ball* yel_0 = &application->balls[1];
    Ball* red_1 = &application->balls[2];
    Ball* yel_1 = &application->balls[3];
    Ball* red_2 = &application->balls[4];
    Ball* yel_2 = &application->balls[5];
    Ball* red_3 = &application->balls[6];
    Ball* yel_3 = &application->balls[7];
    Ball* red_4 = &application->balls[8];
    Ball* yel_4 = &application->balls[9];
    Ball* red_5 = &application->balls[10];
    Ball* yel_5 = &application->balls[11];
    Ball* red_6 = &application->balls[12];
    Ball* yel_6 = &application->balls[13];
    Ball* red_7 = &application->balls[14];
    Ball* yel_7 = &application->balls[15];
    float diameter = black->radius * 2; // These are all the same.
    float radius = black->radius;

    // Set the initial positions.
    //TODO: for now just do the black and white balls.
    Ball_Init(black, screen_width * 0.25, screen_height / 2);
    Ball_Init(white, screen_width * 0.75, screen_height / 2);

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

    application->player_1.score = 0;
    application->player_2.score = 0;
    application->player_1.ball_color = RAYWHITE;
    application->player_2.ball_color = RAYWHITE;
    application->player_1.name = "Player 1";
    application->player_2.name = "Player 2";
    application->current_player = &application->player_1;
    application->next_player = &application->player_2;
    application->pocketed = 0;
    application->state = CUE_AIM;
    TransitionTo(application, CUE_AIM);
}

void Application_Update(Application* app)
{
    float dt = GetFrameTime();

    // Draw game entities.
    DrawTable(app);
    DrawCue(app, dt);
    DrawBalls(app, dt);

    // Tick state machine,
    TickState(app);
}

void CueAimEntryAction(struct Application* app)
{
    Cue_Init(
        &app->cue, 
        (Vector2) { app->white_ball->x, app->white_ball->y }, 
        (Vector2) { app->screen_width / 240.0f, app->screen_width / 8.0f },
        2.0f
    );
}

void CueAimHandler(struct Application* app)
{
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_W))
    {
        Cue_RotateClockwise(&app->cue, dt);
    }
    
    if (IsKeyDown(KEY_S))
    {
        Cue_RotateAnticlockwise(&app->cue, dt);
    }

    if (IsKeyDown(KEY_SPACE))
    {
        // Player is charging up a shot.
        TransitionTo(app, CUE_CHARGE);
    }
}

void CueChargeEntryAction(struct Application* app)
{
    Cue_EnableChargeBar(&app->cue, true);
}

void CueChargeHandler(struct Application* app)
{
    float dt = GetFrameTime();

    Cue_UpdateCharge(&app->cue, dt);

    if (!IsKeyDown(KEY_SPACE))
    {   
        TransitionTo(app, CUE_STRIKE);
    }
}

void CueStrikeEntryAction(struct Application* app)
{
    // Start winding cue back for shot.
    Cue_SetVelocity(&app->cue, 250.0f * app->cue.charge_bar.charge);
}

void CueStrikeHandler(struct Application* app)
{
    float dt = GetFrameTime();
    Cue* cue = &app->cue;

    // One second windup.
    cue->prev_windtime += dt;

    if (cue->prev_windtime >= 0.5f)
    {
        // Now start pushing the cue forward.
        Cue_SetVelocity(cue, -250.0f * cue->charge_bar.charge);
        cue->prev_windtime = 0.0f;
    }

    if (Cue_CheckBallContact(cue, app->white_ball))
    {
        // Ball contact, initiate ball motion state
        TransitionTo(app, BALL_MOTION);
    }
}

void BallMotionEntryAction(struct Application* app)
{
    // TODO
}

void BallMotionHandler(struct Application* app)
{
    float dt = GetFrameTime();
    bool in_motion = false;

    // Update ball physics.
    for (int i = 0; i < 16; i++)
    {
        Ball* ball = &app->balls[i];

        // Check if the ball was pocketed, kaching.
        if (Ball_CheckIfPocketed(ball, &app->table))
        {
            if (ColorCmp(ball->color, BLACK))
            {
                break;
            }

            if (ColorCmp(ball->color, app->current_player->ball_color))
            {
                app->current_player->score++;
            }
            else 
            {
                
                if (!ColorCmp(ball->color, WHITE))
                {
                    // Potted opponent's ball... :(
                    app->next_player->score++;
                }
            }
            continue;
        }

        // Check for collisions with walls of the table.
        Ball_CheckCushionCollision(ball, &app->table);

        // Check for collisions with other balls.
        for (int j = 0; j < 16; j++)
        {
            if (i == j)
                continue;
            Ball* ball2 = &app->balls[j];
            Ball_CheckBallCollision(ball, ball2);
        }

        // Must apply some friction to stop the balls just whacking away indefinitely.
        Ball_ApplyFriction(ball, &app->table);

        // Prevent state transition until all balls have stopped.
        if (Ball_IsMoving(ball))
        {
            in_motion = true;
        }
    }

    // If all balls have stopped moving then prepare for the next shot.
    if (!in_motion)
    {
        TransitionTo(app, CUE_AIM);
    }
}

void FinishedEntryAction(struct Application* app)
{
    // TODO
}

void FinishedHandler(struct Application* app)
{
    // TODO
}