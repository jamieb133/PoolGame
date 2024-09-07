#pragma once

#include "Ball.h"
#include "Table.h"

typedef struct Application {
    Ball balls[16];
    Ball* black_ball;
    Ball* white_ball;
    Table table;
    
    int screen_width, screen_height;
    const char* name;
} Application;

void Application_Init(Application* application, int screen_width, int screen_height, const char* name);
void Application_Run(Application* application);
void Application_Update(Application* application);