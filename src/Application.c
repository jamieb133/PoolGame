#include "Application.h"
#include "GameContext.h"
#include "Logger.h"
#include <raylib.h>
#include <stdbool.h>

void Application_Init(Application* application, int screen_width, int screen_height, const char* name)
{
    InitWindow(screen_width, screen_height, name);
    SetTargetFPS(60);

    Logger_Init();
    GameContext_Init(&application->context, screen_width, screen_height);

    application->name = name;
    application->screen_width = screen_width;
    application->screen_height = screen_height;
    application->running = true;
}

void Application_Update(Application* application)
{
    float const dt = GetFrameTime(); 
    BeginDrawing();
    GameContext_Update(&application->context, dt);
    EndDrawing();
    application->running = !WindowShouldClose();
}

void Application_Deinit(Application* application)
{
    GameContext_Deinit(&application->context);
    CloseWindow();
}

void Application_Reload(Application* application)
{
    // Reset the logger function pointer.
    Logger_Init();
}