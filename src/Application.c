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
