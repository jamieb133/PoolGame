#pragma once

#include "Ball.h"
#include "Table.h"

typedef enum {
    CUE_STRIKE,
    BALL_MOTION,
    FINISHED,

    NUM_STATES
} GameState;

typedef struct {
    Color ball_color;
    int score;
    const char* name;
} Player;

struct Application;

// Periodic update callback for a given state.
typedef void (*StateTick)(struct Application*);

typedef struct Application {
    Ball balls[16];
    Ball* black_ball;
    Ball* white_ball;
    Table table;
    int pocketed;

    GameState state;
    StateTick tick;
    Player player_1, player_2;
    Player *current_player, *next_player;

    int screen_width, screen_height;
    const char* name;
} Application;

void Application_Init(Application* application, int screen_width, int screen_height, const char* name);
void Application_Run(Application* application);
void Application_Update(Application* application);

// State handlers.
void Application_CueStrikeHandler(struct Application* app);
void Application_BallMotionHandler(struct Application* app);
void Application_FinishedHandler(struct Application* app);