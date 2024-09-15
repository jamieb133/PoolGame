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

typedef struct {
    Ball balls[16];
    Ball* black_ball;
    Ball* white_ball;
    Table table;
    Cue cue;
    int pocketed;
    Vector2 screen_size;
    GameState state;
    Player player_1, player_2;
    Player *current_player, *next_player;
    float dt; // Time since previous frame

} GameContext;

// Periodic update callback for a given state.
typedef void (*StateTick)(GameContext*);
typedef void (*StateAction)(GameContext*);

void GameContext_Init(GameContext* context, int const width, int const height);
void GameContext_Update(GameContext* context, float const dt);
void GameContext_Deinit(GameContext* context);
