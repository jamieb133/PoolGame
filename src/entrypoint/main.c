#include "Application.h"
#include <raylib.h>
#include <dlfcn.h>
#include <stdlib.h>

static const char* LIB_NAME = "./libPoolGame_Game.dylib";
static void (*ApplicationInit)(Application*,int,int,const char*);
static void (*ApplicationUpdate)(Application*);

int main()
{
    void* lib_handle = dlopen(LIB_NAME, RTLD_LAZY);
    if (!lib_handle)
    {
        TraceLog(LOG_ERROR, "Failed to load the game logic libary - %s", LIB_NAME);
        exit(1);
    }

    // Reset DLL errors    
    dlerror();

    // Load the game logic functions.
    *(void**)(&ApplicationInit) = dlsym(lib_handle, "Application_Init");

    const char* dlsym_err = dlerror();
    if(dlsym_err)
    {
        TraceLog(LOG_ERROR, "Failed to locate symbol 'Application_Init': %s", dlsym_err);
        dlclose(lib_handle);
        exit(1);
    }

    *(void**)(&ApplicationUpdate) = dlsym(lib_handle, "Application_Update");
    if(dlsym_err)
    {
        TraceLog(LOG_ERROR, "Failed to locate symbol 'Application_Update': %s", dlsym_err);
        dlclose(lib_handle);
        exit(1);
    }

    Application app;

    ApplicationInit(&app, 1200, 600, "Snookered!");
    InitWindow(app.screen_width, app.screen_height, app.name);
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(BLUE);
        ApplicationUpdate(&app);
        EndDrawing();
    }

    CloseWindow();

    dlclose(lib_handle);
}
