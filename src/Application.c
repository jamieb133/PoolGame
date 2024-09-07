#include <raylib.h>
#include <assert.h>
#include <stdlib.h>
#include "Application.h"
#include "Ball.h"

void Application_Init(Application* application, int screen_width, int screen_height, const char* name)
{
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

    // Set the initial positions.
    //TODO: for now just do the black and white balls.
    Ball_Init(application->black_ball, screen_width * 0.25, screen_height / 2);
    Ball_Init(application->white_ball, screen_width * 0.75, screen_height / 2);

    // TODO: initialise cues
}

void Application_Update(Application* application)
{
    float dt = GetFrameTime();

    // Redraw the static game table.
    Table_Draw(&application->table);

    // Update ball physics.
    for (int i = 0; i < 16; i++)
    {
        Ball* ball = &application->balls[i];

        // Check for collisions with walls of the table.
        Ball_CheckCushionCollision(ball, &application->table);

        // Check for collisions with other balls.
        for (int j = 0; j < 16; j++)
        {
            if (i == j)
                continue;
            Ball* ball2 = &application->balls[j];
            Ball_CheckBallCollision(ball, ball2);
        }

        // Must apply some friction to stop the balls just whacking away indefinitely.
        Ball_ApplyFriction(ball, &application->table);

        // Redraw the ball with updated velocity. 
        Ball_Update(ball, dt); 
    }
}

void Application_Run(Application* application)
{
    assert(NULL != application);

    InitWindow(application->screen_width, application->screen_height, application->name);
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(BLUE);

        Application_Update(application);

        EndDrawing();
    }

    CloseWindow();
}
