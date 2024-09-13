#include "Application.h"
#include <raylib.h>

int main()
{
    Application app;

    Application_Init(&app, 1200, 600, "Snookered!");
    InitWindow(app.screen_width, app.screen_height, app.name);
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(BLUE);
        Application_Update(&app);
        EndDrawing();
    }

    CloseWindow();
}
