#ifndef GAME_H
#define GAME_H

//Configuration settings for the game
typedef struct
{
    int rows;
    int cols;
    int die_sides;
    int max_steps;
    int exact_win;
} GameConfig;

//Board structure holding the jump logic (snakes/ladders)
typedef struct
{
    int *jumps;  //Each index may contain a jump offset
    int size;
} Board;

//Result of a single game simulation
typedef struct
{
    int rolls;
    int *roll_sequence;
    int max_rolls;
} SimulationResult;

//Functions to manage the game
Board *create_board(int rows, int cols);
void add_ladder(Board *board, int start, int end);
void add_snake(Board *board, int start, int end);
SimulationResult simulate_game(Board *board, GameConfig config, int *snake_hits, int *ladder_hits);
void free_board(Board *board);

#endif
