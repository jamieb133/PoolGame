#pragma once

#include "GameContext.h"

typedef struct Application {
    int screen_width, screen_height;
    const char* name;
    bool running;
    GameContext context;
} Application;

void Application_Init(Application* application, int screen_width, int screen_height, const char* name);
void Application_Update(Application* application);
void Application_Deinit(Application* application);
void Application_Reload(Application* application);
