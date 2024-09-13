#pragma once

#include "Ball.h"
#include "Table.h"
#include "Cue.h"

typedef enum {
    CUE_AIM,
    CUE_CHARGE,
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
typedef void (*StateAction)(struct Application*);

typedef struct Application {
    Ball balls[16];
    Ball* black_ball;
    Ball* white_ball;
    Table table;
    Cue cue;
    int pocketed;

    GameState state;
    StateTick state_handler;
    Player player_1, player_2;
    Player *current_player, *next_player;

    int screen_width, screen_height;
    const char* name;
} Application;

void Application_Init(Application* application, int screen_width, int screen_height, const char* name);
void Application_Update(Application* application);
